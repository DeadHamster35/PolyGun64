#ifndef MemoryH
#define MemoryH

#include <ultra64.h>
#include "common.h"

extern uint RawBufferPointer;
extern uint SegmentTable[];

extern void SetSegment(uint SegmentID, uint RAMAddress);
extern uint GetRealAddress(uint RSPAddress);
extern void StoreRSPSegments();
extern void mio0decode(unsigned char * Source,unsigned char * Target);
extern uint DecompressData(uint Source, uint CompressionLength);
#endif

