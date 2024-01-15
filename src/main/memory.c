
#include <ultra64.h>
#include "gfx.h"
#include "common.h"
#include "simple.h"
#include "compressionbuffer.h"


uint SegmentTable[16];

uint RawBufferPointer;


extern void slidec1(unsigned char * Source,unsigned char * Target);

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

uint DecompressData(uint Source, uint CompressionLength)
{
    uint LoadAddress = RawBufferPointer;

    LoadDMA((char*)Source, (char*)&CompressionBuffer, CompressionLength);

    slidec1((char*)&CompressionBuffer, (char*)LoadAddress);

    uint Size = *(uint*)(((uint)&CompressionBuffer) + 4);
    RawBufferPointer += Align32(Size);

    return LoadAddress;
}