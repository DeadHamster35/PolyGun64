#ifndef MemoryH
#define MemoryH

#include <ultra64.h>
#include "common.h"

extern void SetSegment(uint SegmentID, uint RAMAddress);
extern uint GetRealAddress(uint RSPAddress);
extern void StoreRSPSegments();
#endif

