/*====================================================================
 * gfx.c
 *
 * Synopsis:
 *
 * This code implements the application graphics stuff
 * 
 * 
 * 
 * Copyright 1993, Silicon Graphics, Inc.
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Silicon Graphics,
 * Inc.; the contents of this file may not be disclosed to third
 * parties, copied or duplicated in any form, in whole or in part,
 * without the prior written permission of Silicon Graphics, Inc.
 *
 * RESTRICTED RIGHTS LEGEND:
 * Use, duplication or disclosure by the Government is subject to
 * restrictions as set forth in subdivision (c)(1)(ii) of the Rights
 * in Technical Data and Computer Software clause at DFARS
 * 252.227-7013, and/or in similar or successor clauses in the FAR,
 * DOD or NASA FAR Supplement. Unpublished - rights reserved under the
 * Copyright Laws of the United States.
 *====================================================================*/

#include <ultralog.h>

#include "common.h"
#include "gfx.h"
#include "simple.h"
#include "memory.h"
#include "player.h"
#include "levels.h"

/*
 * graphics globals
 */

extern OSSched         sc;
extern OSMesgQueue     *sched_cmdQ;
extern GFXInfo         gInfo[];

extern s8    logoPos_x;
extern s8    logoPos_y;
extern s8    logoPos_z;
extern f32   logoScale_x;
extern f32   logoScale_y;
extern f32   logoScale_z;
extern f32   logoVeloc;




static u32          framecount;

static char         *staticSegment = 0;

#ifndef DEBUGBARS
extern OSTime      lastTime;
#endif

void DrawScene(Dynamic *dynamicp, int CameraIndex);


void initGFX(void) 
{    
    extern char _gfxdlistsSegmentEnd[];

    
    u32 len = (u32)(_staticSegmentRomEnd - _staticSegmentRomStart);

    staticSegment = _gfxdlistsSegmentBssStart;
    LoadDMA(_staticSegmentRomStart, staticSegment, len);
    
    gInfo[0].msg.gen.type = OS_SC_DONE_MSG;
    gInfo[0].cfb = cfb_16_a;
    gInfo[1].msg.gen.type = OS_SC_DONE_MSG;
    gInfo[1].cfb = cfb_16_b;

    /* The Vi manager was started by scheduler by this point in time */
    osViSetSpecialFeatures(OS_VI_DITHER_FILTER_ON);

    
}



void createGfxTask(GFXInfo *i) 
{
    static int firsttime = 1;
    Dynamic *dynamicp;
    OSScTask *t;
    
    /**** pointers to build the display list. ****/
    dynamicp = &i->dp;
    glistp   = i->dp.glist;

    SetSegment(0,0);
    SetSegment(STATIC_SEGMENT,osVirtualToPhysical(staticSegment));
    SetSegment(DYNAMIC_SEGMENT, osVirtualToPhysical(dynamicp));




    StoreRSPSegments();

    /**** Graphics pipeline state initialization ****/
    gSPDisplayList(glistp++, setup_rspstate);
    if (firsttime) {
        
        osViSetMode(&osViModeTable[OS_VI_NTSC_HAF1]);
        gSPDisplayList(glistp++, rdpstateinit_dl);
        
        osViSetYScale(0.7f);
        osViSetXScale(0.8f);
	    firsttime = 0;
    }

    gSPDisplayList(glistp++, setup_rdpstate);
    
    /**** clear zbuffer, z = max z, dz = 0  ****/
    gDPSetDepthImage(glistp++, osVirtualToPhysical(zbuffer));
    gDPPipeSync(glistp++);
    gDPSetCycleType(glistp++, G_CYC_FILL);
    gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD,
		     osVirtualToPhysical(zbuffer));
    gDPSetFillColor(glistp++, (GPACK_ZDZ(G_MAXFBZ,0) << 16 |
			       GPACK_ZDZ(G_MAXFBZ,0)));
    gDPFillRectangle(glistp++, 0, 0, SCREEN_WD-1, SCREEN_HT-1);
	
    /**** Clear framebuffer cvg = FULL or 1  ****/
    gDPPipeSync(glistp++);
    gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD,
		     osVirtualToPhysical(i->cfb));
    /* clearcolor is 32-bits (2 pixels wide) because fill mode
     * does two pixels at a time.
     */
    gDPSetFillColor(glistp++, (GPACK_RGBA5551(0, 0, 0, 1) << 16 | 
			       GPACK_RGBA5551(0, 0, 0, 1)));
    gDPFillRectangle(glistp++, 0, 0, SCREEN_WD-1, SCREEN_HT-1);

    gDPPipeSync(glistp++);
    gDPSetCycleType(glistp++, G_CYC_1CYCLE); 


    /**** Draw objects */


    DrawScene(dynamicp, 0);



    

    
    /**** Put an end on the top-level display list  ****/
    gDPFullSync(glistp++);
    gSPEndDisplayList(glistp++);

    /* Flush the dynamic segment */
    osWritebackDCache(&i->dp, (s32)glistp - (s32)&i->dp);

    /* build graphics task */

    t = &i->task;
    t->list.t.data_ptr = (u64 *) dynamicp->glist;
    t->list.t.data_size = (s32)(glistp - dynamicp->glist) * sizeof (Gfx);
    t->list.t.type = M_GFXTASK;
    t->list.t.flags = OS_TASK_LOADABLE;
    t->list.t.ucode_boot = (u64 *)rspbootTextStart;
    t->list.t.ucode_boot_size = ((s32) rspbootTextEnd - 
				 (s32) rspbootTextStart);
    t->list.t.ucode = (u64 *) gspF3DEX2_fifoTextStart;
    t->list.t.ucode_data = (u64 *) gspF3DEX2_fifoDataStart;
    t->list.t.ucode_data_size = SP_UCODE_DATA_SIZE;
    t->list.t.dram_stack = (u64 *) dram_stack;
    t->list.t.dram_stack_size = SP_DRAM_STACK_SIZE8;
    t->list.t.output_buff = (u64 *) rdp_output;
    t->list.t.output_buff_size = (u64 *) rdp_output + 0x2fc0;
    t->list.t.yield_data_ptr = (u64 *) gfxYieldBuf;
    t->list.t.yield_data_size = OS_YIELD_DATA_SIZE;

    t->next     = 0;                   /* paranoia */
    t->flags	= (OS_SC_NEEDS_RSP | OS_SC_NEEDS_RDP | OS_SC_LAST_TASK |
		   OS_SC_SWAPBUFFER);
    t->msgQ     = &gfxFrameMsgQ;       /* reply to when finished */
    t->msg      = (OSMesg)&i->msg;     /* reply with this message */
    t->framebuffer = (void *)i->cfb;
#ifndef DEBUGBARS
    t->totalTime = 0;
#endif
    osSendMesg(sched_cmdQ, (OSMesg) t, OS_MESG_BLOCK); 
    
    framecount++;
}


void DrawLevelScene(Dynamic *dynamicp, int CameraIndex)
{
    
    Player* LocalPlayer =       (Player*)&GamePlayers[CameraIndex];
    PGCamera* LocalCamera =     (PGCamera*)&GamePlayers[CameraIndex].Camera;

    

    u16		   LevelNormal, SkyNormal, FPNormal;

    guPerspective(&dynamicp->BGMap.Projection, &SkyNormal,
		  LocalCamera->FOVY, 480.0f/320.0f, 1.0f, 500.0f, 1.0);          
    guLookAt(&dynamicp->BGMap.Viewing, 
	     0, 100.0f, 0,
	     0, 0, 0,
	     0, 0, 1);
    
    gSPPerspNormalize(glistp++, SkyNormal);

    guTranslate(&dynamicp->BGMap.Translation, 0.0f, 0.0f, 0.0f);
    guScale(&dynamicp->BGMap.Scale, 1.0f, 1.0f, 1.0f);    
    guRotate(&dynamicp->BGMap.Rotation, 0.0f, 0.0F, 0.0F, 1.0);   
    


    gSPMatrix(glistp++, &dynamicp->BGMap.Projection, 
	       G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);           
    gSPMatrix(glistp++, &dynamicp->BGMap.Viewing, 
	       G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);
    gSPMatrix(glistp++, &dynamicp->BGMap.Translation, 
	      G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPMatrix(glistp++, &dynamicp->BGMap.Scale, 
	      G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);
    gSPMatrix(glistp++, &dynamicp->BGMap.Rotation, 
	      G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);

           
    gSPDisplayList(glistp++, bg_dl);
    

    

    guPerspective(&dynamicp->LevelMap.Projection, &LevelNormal,
		  LocalCamera->FOVY, 480.0f/320.0f, LocalCamera->Near, LocalCamera->Far, 1.0);
    gSPPerspNormalize(glistp++, LevelNormal);

    guLookAt(&dynamicp->LevelMap.Viewing, 
        LocalCamera->Location.Position[0], LocalCamera->Location.Position[1], LocalCamera->Location.Position[2],
        LocalCamera->LookAt[0], LocalCamera->LookAt[1], LocalCamera->LookAt[2],
        LocalCamera->UpVector[0], LocalCamera->UpVector[1], LocalCamera->UpVector[2]);
    
    guTranslate(&dynamicp->LevelMap.Translation, 0.0f, 0.0f, 0.0f);
    guScale(&dynamicp->LevelMap.Scale, 1.0f, 1.0f, 1.0f);    
    guRotate(&dynamicp->LevelMap.Rotation, 0.0f, 0.0F, 1.0F, 0.0);

    /* Setup model matrix */
    gSPMatrix(glistp++, &dynamicp->LevelMap.Projection, 
	       G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPMatrix(glistp++, &dynamicp->LevelMap.Viewing, 
	       G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);
    gSPMatrix(glistp++, &dynamicp->LevelMap.Translation, 
	      G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPMatrix(glistp++, &dynamicp->LevelMap.Scale, 
	      G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);
    gSPMatrix(glistp++, &dynamicp->LevelMap.Rotation, 
	      G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);
    
    /* Draw the level */
    gSPDisplayList(glistp++, TableAddress[LevelIndex]);

}



void DrawMenu(Dynamic *dynamicp, int CameraIndex)
{
    
    Player* LocalPlayer =       (Player*)&GamePlayers[CameraIndex];
    PGCamera* LocalCamera =     (PGCamera*)&GamePlayers[CameraIndex].Camera;

    

    u16		   LevelNormal, SkyNormal, FPNormal;

    guPerspective(&dynamicp->BGMap.Projection, &SkyNormal,
		  LocalCamera->FOVY, 480.0f/320.0f, 1.0f, 500.0f, 1.0);          
    guLookAt(&dynamicp->BGMap.Viewing, 
	     0, 100.0f, 0,
	     0, 0, 0,
	     0, 0, 1);
    
    gSPPerspNormalize(glistp++, SkyNormal);

    guTranslate(&dynamicp->BGMap.Translation, 0.0f, 0.0f, 0.0f);
    guScale(&dynamicp->BGMap.Scale, 1.0f, 1.0f, 1.0f);    
    guRotate(&dynamicp->BGMap.Rotation, 0.0f, 0.0F, 0.0F, 1.0);   
    


    gSPMatrix(glistp++, &dynamicp->BGMap.Projection, 
	       G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);           
    gSPMatrix(glistp++, &dynamicp->BGMap.Viewing, 
	       G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);
    gSPMatrix(glistp++, &dynamicp->BGMap.Translation, 
	      G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPMatrix(glistp++, &dynamicp->BGMap.Scale, 
	      G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);
    gSPMatrix(glistp++, &dynamicp->BGMap.Rotation, 
	      G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);

           
    gSPDisplayList(glistp++, menubg_dl);
    


}
void DrawScene(Dynamic *dynamicp, int CameraIndex)
{


    static float   logo_theta = 0;
#ifndef DEBUGBARS
    u32      timeLen;
#endif

    switch(GameSequence)
    {
        case(MENUSEQUENCE):
        {
            DrawMenu(dynamicp, CameraIndex);
            break;
        }
        case(LEVELSEQUENCE):
        {
            DrawLevelScene(dynamicp, CameraIndex);
            break;
        }
        
    }
    



#ifndef DEBUGBARS   /* draw the performance bar */
#if 0
#define USECS_PER_FRAME    16666  /* assuming a frame rate of 60fps */
#define USECS_PER_PIXEL    150
#define PIXELS_PER_FRAME   USECS_PER_FRAME/USECS_PER_PIXEL
#define LEFT_OFFSET        40
#define MARK_TOP           210
#define BAR_TOP            215
#define BAR_BOTTOM         220
#define MARK_BOTTOM        225
#define MARK_WIDTH         2

    gDPPipeSync(glistp++);

    gDPSetCycleType(glistp++, G_CYC_1CYCLE);
    gDPSetPrimColor(glistp++, 0, 0, 200, 0, 120, 255);
    gDPSetCombineMode(glistp++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    gDPSetRenderMode(glistp++, G_RM_OPA_SURF, G_RM_OPA_SURF2);

    timeLen =(u32) (OS_CYCLES_TO_USEC(lastTime) / USECS_PER_PIXEL);
    gDPFillRectangle(glistp++, LEFT_OFFSET, BAR_TOP, 
		     LEFT_OFFSET + timeLen, BAR_BOTTOM);
    gDPFillRectangle(glistp++, LEFT_OFFSET+PIXELS_PER_FRAME, MARK_TOP, 
		     LEFT_OFFSET+PIXELS_PER_FRAME+MARK_WIDTH, MARK_BOTTOM);
    gDPFillRectangle(glistp++, LEFT_OFFSET+(2*PIXELS_PER_FRAME), MARK_TOP, 
		     LEFT_OFFSET+(2*PIXELS_PER_FRAME)+MARK_WIDTH, MARK_BOTTOM);
#endif
#endif
}
