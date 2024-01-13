#ifndef ControlH
#define ControlH

#include <ultra64.h>
#include "common.h"

extern OSContStatus statusdata[MAXCONTROLLERS];
extern OSContPad ControllerArray[MAXCONTROLLERS];
extern OSScMsg controllermsg;
extern u16 lastButArray[MAXCONTROLLERS];
#endif