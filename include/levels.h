#ifndef LevelH
#define LevelH

#include <ultra64.h>
#include "common.h"

extern LevelHeaderStruct LevelHeader;
extern void LoadLevelData();



extern int LevelIndex;

extern void LoadHeader();
extern void LoadLevelData();





 
extern char _CityscapeSeg4[], _CityscapeSeg4END[];
extern char _CityscapeSeg5[], _CityscapeSeg5END[];
extern char _CityscapeSeg6[], _CityscapeSeg6END[];

extern char _MansionSeg4[], _MansionSeg4END[];
extern char _MansionSeg5[], _MansionSeg5END[];
extern char _MansionSeg6[], _MansionSeg6END[];


extern char _BobombSeg4[], _BobombSeg4END[];
extern char _BobombSeg5[], _BobombSeg5END[];
extern char _BobombSeg6[], _BobombSeg6END[];

extern uint Segment4Array[];
extern uint Segment5Array[];
extern uint Segment6Array[];

extern uint TableAddress[];

#endif

