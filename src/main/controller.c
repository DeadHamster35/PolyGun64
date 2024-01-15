/**********************************************************************
 * Controller.c
 *
 * This file contains the code for interfacing the controllers to the
 * game. Basically, when a controller read has completed, check to
 * see what buttons have been pressed, and change some external state
 * variables to let the application know what has happened.
 *
 * Copyright 1995, Silicon Graphics, Inc.
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
 *********************************************************************/

#include <ultralog.h>
#include <sched.h>
#include "simple.h"
#include "levels.h"
#include "audio.h"

#define X_SCALE_INCR 0.05f
#define Y_SCALE_INCR 0.05f
#define X_SCALE_MIN 0.2f
#define X_SCALE_MAX 4.0f
#define Y_SCALE_MIN 0.2f
#define Y_SCALE_MAX 3.0f
#define VELOCITY_INCR 0.2f

/****  Logo values ****/
s8 logoPos_x = 0;
s8 logoPos_y = 0;
s8 logoPos_z = 0;
f32 logoScale_x = 1.0f;
f32 logoScale_y = 1.0f;
f32 logoScale_z = 1.0f;
f32 logoVeloc = 4.0f;

/****  Variables shared with other files ****/
u8 validcontrollers = 0;

/**** variables used by this file for handling the controllers ****/
OSContStatus statusdata[MAXCONTROLLERS];
OSContPad ControllerArray[MAXCONTROLLERS];
OSScMsg controllermsg;
u16 lastButArray[MAXCONTROLLERS];

/**********************************************************************
 *
 * Routine for initializing the controllers. After initialized, set the
 * controller interrupt to be posted to the gfxFrameMsgQ, used by the
 * gameproc in simple.c.
 *
 *********************************************************************/
void initCntrl(void)
{
    OSMesgQueue serialMsgQ;
    OSMesg serialMsg;
    s32 i;

    osCreateMesgQueue(&serialMsgQ, &serialMsg, 1);
    osSetEventMesg(OS_EVENT_SI, &serialMsgQ, (OSMesg)1);
    if((i = osContInit(&serialMsgQ, &validcontrollers, &statusdata[0])) != 0)
    {
        PRINTF("Failure initing controllers\n");
    }

    /**** Set up message and queue, for read completion notification ****/
    controllermsg.type = SIMPLE_CONTROLLER_MSG;

    osSetEventMesg(OS_EVENT_SI, &gfxFrameMsgQ, (OSMesg)&controllermsg);
}

/**********************************************************************
 *
 * Called after the controller read has completed. Check the information
 * from the read, and set state variables accordingly. If you were only
 * interested in one button press at a time, you could use a switch or
 * a jump table. If you are going to respond to multiple button presses,
 * you will need to do a series of if's. In this example we only use
 * new button presses, ignoring buttons held down. Do this by exclusive
 * or'ing the lastbutton with pad->button, and then and pad->button
 * with the result to only get the button downs, and ignore button ups.
 *
 *********************************************************************/
short LoadLevelToggled;
void UpdateController(void)
{
    OSContPad *pad;
    u16 i, newbutton;
    u16 firstCntrl = 1;
    PRINTF("CONTROLLER");
    osContGetReadData(ControllerArray);

    if (LoadLevelToggled)
    {
        while (RenderProcessing)
        {
        };

        LoadHeader();
        LoadLevelData();                
        RenderEnable = 1;
        LoadLevelToggled = 0;
    }

    for (i = 0; i < MAXCONTROLLERS; i++)
    {
        if ((validcontrollers >> i) & 1)
        {
            pad = &ControllerArray[i];
            /* ignore controllers with errors */
            if (pad->errno)
                continue;
            /* get bits that have changed, ignore button ups */
            newbutton = pad->button ^ lastButArray[i];
            newbutton &= pad->button;
            lastButArray[i] = pad->button;
        }
    }
}
