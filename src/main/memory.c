
#include <ultra64.h>
#include "gfx.h"
#include "common.h"
#include "simple.h"


uint SegmentTable[16];

void SetSegment(uint SegmentID, uint RAMAddress)
{
    SegmentTable[SegmentID] = K0_TO_PHYS(RAMAddress);
}

uint GetRealAddress(uint RSPAddress)
{
	uint RSPNumber = SegmentNumber(RSPAddress);
	uint RSPOffset = SegmentOffset(RSPAddress);
	return(PHYS_TO_K0(SegmentTable[RSPNumber] + RSPOffset));
}

void StoreRSPSegments()
{
    for (int ThisRSP = 0; ThisRSP < 16; ThisRSP++)
    {
        gSPSegment(glistp++, ThisRSP, SegmentTable[ThisRSP]);
    }
}

