
#include <ultra64.h>
#include "gfx.h"
#include "common.h"
#include "simple.h"
#include "math.h"


extern void TransformMatrix(AffineMtx InputMatrix, Vector Position, Vector Look, Vector Up)
{
    float len;
    float xUp	, yUp	, zUp	;
    float xLook , yLook , zLook ;
    float xRight, yRight, zRight;

    xUp = Up[0];
    yUp = Up[1];
    zUp = Up[2];

    xLook = Look[0] - Position[0];
    yLook = Look[1] - Position[1];
    zLook = Look[2] - Position[2];
    len = -1.0 / sqrtf (xLook*xLook + yLook*yLook + zLook*zLook);
    xLook *= len;
    yLook *= len;
    zLook *= len;

    /* Right = Up x Look */

    xRight = yUp * zLook - zUp * yLook;
    yRight = zUp * xLook - xUp * zLook;
    zRight = xUp * yLook - yUp * xLook;
    len = 1.0 / sqrtf (xRight*xRight + yRight*yRight + zRight*zRight);
    xRight *= len;
    yRight *= len;
    zRight *= len;

    /* Up = Look x Right */

    xUp = yLook * zRight - zLook * yRight;
    yUp = zLook * xRight - xLook * zRight;
    zUp = xLook * yRight - yLook * xRight;
    len = 1.0 / sqrtf (xUp*xUp + yUp*yUp + zUp*zUp);
    xUp *= len;
    yUp *= len;
    zUp *= len;

    InputMatrix[0][0] = xRight;
    InputMatrix[1][0] = yRight;
    InputMatrix[2][0] = zRight;
    InputMatrix[3][0] = -((float)Position[0] * xRight + (float)Position[1] * yRight + (float)Position[2] * zRight);

    InputMatrix[0][1] = xUp;
    InputMatrix[1][1] = yUp;
    InputMatrix[2][1] = zUp;
    InputMatrix[3][1] = -((float)Position[0] * xUp + (float)Position[1] * yUp + (float)Position[2] * zUp);

    InputMatrix[0][2] = xLook;
    InputMatrix[1][2] = yLook;
    InputMatrix[2][2] = zLook;
    InputMatrix[3][2] = -((float)Position[0] * xLook + (float)Position[1] * yLook + (float)Position[2] * zLook);

    InputMatrix[0][3] = 0.0f;
    InputMatrix[1][3] = 0.0f;
    InputMatrix[2][3] = 0.0f;
    InputMatrix[3][3] = 1.0f;
}
