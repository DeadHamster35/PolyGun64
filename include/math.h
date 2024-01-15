#ifndef MathH
#define MathH

#include <ultra64.h>
#include "common.h"

extern void TransformMatrix(AffineMtx InputMatrix, Vector Position, Vector Look, Vector Up);
extern void AlignZVector(Vector Target, short Angle);
extern void AlignXVector(Vector Target, short Angle);
extern void AlignYVector(Vector Target, short Angle);

extern float sint(ushort angle);
extern float cost(ushort angle);

#endif

