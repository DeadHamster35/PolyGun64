#ifndef CommonH
#define CommonH

#include <ultra64.h>
#define DEG1 0xB9

#define	SegmentOffset(a)	((unsigned int)(a) & 0x00ffffff)
#define	SegmentNumber(a)	(((unsigned int)(a) << 4) >> 28)
#define	SegmentAddress(num, off)	(((num) << 24) + (off))


/* Buttons */

#define CONT_A      0x8000
#define CONT_B      0x4000
#define CONT_G	    0x2000
#define CONT_START  0x1000
#define CONT_UP     0x0800
#define CONT_DOWN   0x0400
#define CONT_LEFT   0x0200
#define CONT_RIGHT  0x0100
#define CONT_L      0x0020
#define CONT_R      0x0010
#define CONT_E      0x0008
#define CONT_D      0x0004
#define CONT_C      0x0002
#define CONT_F      0x0001

/* Nintendo's official button names */

#define BTN_A	        CONT_A
#define BTN_B	        CONT_B
#define BTN_L		    CONT_L
#define BTN_R		    CONT_R
#define BTN_Z		    CONT_G
#define BTN_START	    CONT_START
#define BTN_DUP		    CONT_UP
#define BTN_DLEFT		CONT_LEFT
#define BTN_DRIGHT		CONT_RIGHT
#define BTN_DDOWN		CONT_DOWN
#define BTN_CUP	        CONT_E
#define BTN_CLEFT	    CONT_C
#define BTN_CRIGHT	    CONT_F
#define BTN_CDOWN	    CONT_D

typedef unsigned int uint;
typedef float AffineMtx[4][4];
typedef float Vector[3];
typedef short SVector[3];

typedef struct {
    
    Vector      Position;
    Vector      VelocityFront;
    Vector      VelocitySide;
    SVector     Angle;
    short       Radius;
} Locate;

typedef struct {
    Locate      Location;
    Vector      LookAt;
    Vector      UpVector;
    short       FOVX, FOVY;
    short       Near,Far;    
} PGCamera;



typedef struct{
    Locate      Location;    
    PGCamera    Camera;
} Player;

#endif

