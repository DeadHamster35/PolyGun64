#ifndef CollisionH
#define CollisionH

#include <ultra64.h>
#include "common.h"

extern void BuildCollisionBuffer(uint Address);


extern CollisionTri CollisionBuffer[MAXTRI];
extern uint CollisionCount;
extern void CollisionCheck(int Index);
#endif

