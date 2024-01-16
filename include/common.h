#ifndef CommonH
#define CommonH

#include <ultra64.h>
#define DEG1 0xB9



#define MENUSEQUENCE 0
#define LEVELSEQUENCE 1

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


#define	Align128(n)		(((uint)(n)+15)&0xfffffff0)
#define	Align64(n)		(((uint)(n)+7)&0xfffffff8)
#define	Align32(n)		(((uint)(n)+3)&0xfffffffc)
#define	Align16(n)		(((uint)(n)+1)&0xfffffffe)

typedef unsigned int uint;
typedef unsigned short ushort;
typedef float AffineMtx[4][4];
typedef float Vector[3];
typedef short SVector[3];

#define MAXTRI 4096
#define XAXISVECTOR 0
#define YAXISVECTOR 1
#define ZAXISVECTOR 2
#define WTFAXISVECTOR 3



#define MAXSPEED 3.5f
#define MAXLOOK 25

typedef struct {

    Vector  Center;
    float   Radius;

} Sphere;
typedef struct {

    float   BoundingMax[3];
    float   BoundingMin[3];
    short   NormalDirection, SurfaceType;
    float   VectorDistance;
    Vector  Vertex[3];
    Vector  Normal;

} CollisionTri;



typedef struct {
    
    Vector      Position;
    Vector      LastPosition;
    Vector      VelocityFront;
    Vector      VelocitySide;
    Vector      VelocityTotal;
    SVector     Angle;
    short       Radius;
} Locate;

typedef struct {
    Locate      Location;
    Vector      LookAt;
    Vector      UpVector;
    short       FOVY, LastHit;
    short       Near,Far;    
} PGCamera;



typedef struct{
    Locate      Location;    
    PGCamera    Camera;
} Player;

typedef struct {

    uint    Segment4ROM;
    uint    Segment5ROM;
    uint    Segment6ROM;

    uint    Segment4SizeCompressed;
    uint    Segment5SizeCompressed;
    uint    Segment6SizeCompressed;
} LevelHeaderStruct;


#endif

