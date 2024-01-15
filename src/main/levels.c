
#include <ultra64.h>
#include "gfx.h"
#include "common.h"
#include "simple.h"
#include "math.h"
#include "memory.h"
#include "compressionbuffer.h"
#include "levels.h"


LevelHeaderStruct LevelHeader;

int LevelIndex;


uint Segment4Array[] = {(uint)&_CityscapeSeg4, (uint)&_MansionSeg4, (uint)&_BobombSeg4};
uint Segment5Array[] = {(uint)&_CityscapeSeg5, (uint)&_MansionSeg5, (uint)&_BobombSeg5};
uint Segment6Array[] = {(uint)&_CityscapeSeg6, (uint)&_MansionSeg6, (uint)&_BobombSeg6};

uint TableAddress[] = {0x060104B8, 0x06011B20, 0x0600E698};

uint Seg4Ends[] = 
{
    (uint)&_CityscapeSeg4END,
    (uint)&_MansionSeg4END,
    (uint)&_BobombSeg4END
};
uint Seg5Ends[] = 
{
    (uint)&_CityscapeSeg5END,
    (uint)&_MansionSeg5END,
    (uint)&_BobombSeg5END
};
uint Seg6Ends[] = 
{
    (uint)&_CityscapeSeg6END,
    (uint)&_MansionSeg6END,
    (uint)&_BobombSeg6END
};

void LoadHeader()
{
    LevelHeader.Segment4ROM = Segment4Array[LevelIndex];
    LevelHeader.Segment5ROM = Segment5Array[LevelIndex];
    LevelHeader.Segment6ROM = Segment6Array[LevelIndex];

    LevelHeader.Segment4SizeCompressed = Seg4Ends[LevelIndex] - Segment4Array[LevelIndex];
    LevelHeader.Segment5SizeCompressed = Seg5Ends[LevelIndex] - Segment5Array[LevelIndex];
    LevelHeader.Segment6SizeCompressed = Seg6Ends[LevelIndex] - Segment6Array[LevelIndex];
}

void LoadLevelData()
{
    uint ReturnAddress;

    RawBufferPointer = (uint)&RawBuffer;

    *(uint*)(0x803FFFF0) = (uint)&CompressionBuffer;
    *(uint*)(0x803FFFF4) = (uint)RawBufferPointer;
    *(uint*)(0x803FFFF8) = (uint)&RawBuffer;
    *(uint*)(0x803FFFFC) = (uint)(&SegmentTable);

    ReturnAddress = DecompressData(LevelHeader.Segment4ROM, LevelHeader.Segment4SizeCompressed);
    SetSegment(4, ReturnAddress);
    
    *(uint*)(0x803FFFF4) = (uint)RawBufferPointer;

    ReturnAddress = DecompressData(LevelHeader.Segment5ROM, LevelHeader.Segment5SizeCompressed);
    SetSegment(5, ReturnAddress);
    
    *(uint*)(0x803FFFF4) = (uint)RawBufferPointer;

    ReturnAddress = DecompressData(LevelHeader.Segment6ROM, LevelHeader.Segment6SizeCompressed);
    SetSegment(6, ReturnAddress);
    
    *(uint*)(0x803FFFF4) = (uint)RawBufferPointer;

    
}