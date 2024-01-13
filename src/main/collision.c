#include <ultra64.h>
#include <ultralog.h>
#include <sched.h>
#include <stdbool.h>
#include "math.h"
#include "simple.h"
//#include "player.h"
#include "common.h"
#include "memory.h"



#ifdef Collision
CollisionTri CollisionBuffer[MAXTRI];
uint CollisionCount;

typedef struct
{
    uint CodeA;
    uint CodeB;
} F3DCode;

typedef struct
{
    uint    F3DAddress;
    char    Material;
    char    SectionID;
    short   Flag;
} CollisionEntry;


Vector VertexCache[64];



extern void SaveNormal();
extern void SaveAxis();


void LoadCache(F3DCode* OpCode)
{
    int Count = ((OpCode->CodeA >> 12) & 0xFF);
    int Index = ((OpCode->CodeA >> 1) & 0x7F);
    Index -= Count;

    for (int Start = 0; Start < Count; Start++)
    {
        Vtx* VTarget = (Vtx*)(GetRealAddress(OpCode->CodeB) + (Start * sizeof(Vtx)));
        VertexCache[Index + Start][0] = VTarget->v.ob[0];
        VertexCache[Index + Start][1] = VTarget->v.ob[1];
        VertexCache[Index + Start][2] = VTarget->v.ob[2];        
    }
}


void Save1Triangle(F3DCode* OpCode)
{
    if (CollisionCount >= MAXTRI)
    {
        return;
    }

    short Index[3];

    Index[0] = (OpCode->CodeA & 0xFF);
    Index[1] = ((OpCode->CodeA >> 8) & 0xFF);
    Index[2] = ((OpCode->CodeA >> 16) & 0xFF);

    for (int ThisVector = 0; ThisVector < 3; ThisVector++)
    {
        CollisionBuffer[CollisionCount].BoundingMax[ThisVector] = -99999999999.0f;
        CollisionBuffer[CollisionCount].BoundingMin[ThisVector] = 99999999999.0f;
    }

    for (int ThisVert = 0; ThisVert < 3; ThisVert++)
    {
        for (int ThisVector = 0; ThisVector < 3; ThisVector++)
        {
            CollisionBuffer[CollisionCount].Geometry.Vertex[ThisVert][ThisVector] = VertexCache[Index[ThisVert]][ThisVector];
            


            if (VertexCache[Index[ThisVert]][ThisVector] < CollisionBuffer[CollisionCount].BoundingMin[ThisVector])
            {
                CollisionBuffer[CollisionCount].BoundingMin[ThisVector] = VertexCache[Index[ThisVert]][ThisVector];
            }

            if (VertexCache[Index[ThisVert]][ThisVector] > CollisionBuffer[CollisionCount].BoundingMax[ThisVector])
            {
                CollisionBuffer[CollisionCount].BoundingMax[ThisVector] = VertexCache[Index[ThisVert]][ThisVector];
            }
        }       

        
        
    }

    SaveNormal();
    SaveAxis();

    
    
    CollisionCount++;

}


void Save2Triangle(F3DCode* OpCode)
{

    
    short Index[3];

    if (CollisionCount >= MAXTRI)
    {
        return;
    }

    Index[0] = (OpCode->CodeA & 0xFF) / 2;
    Index[1] = ((OpCode->CodeA >> 8) & 0xFF)  / 2; 
    Index[2] = ((OpCode->CodeA >> 16) & 0xFF)  / 2;

    PRINTF("Triangle # %d \n", (int)CollisionCount);
    PRINTF("Triangle Index0 X %d \n", (int)Index[0]);
    PRINTF("Triangle Index1 Y %d \n", (int)Index[1]);
    PRINTF("Triangle Index2 Z %d \n", (int)Index[2]);
    

    for (int ThisVector = 0; ThisVector < 3; ThisVector++)
    {
        CollisionBuffer[CollisionCount].BoundingMax[ThisVector] = -99999999999.0f;
        CollisionBuffer[CollisionCount].BoundingMin[ThisVector] = 99999999999.0f;
    }

    for (int ThisVert = 0; ThisVert < 3; ThisVert++)
    {
        PRINTF("Triangle Vertex # %d \n", (int)ThisVert);
        for (int ThisVector = 0; ThisVector < 3; ThisVector++)
        {
            CollisionBuffer[CollisionCount].Geometry.Vertex[ThisVert][ThisVector] = VertexCache[Index[ThisVert]][ThisVector];
            

            
            PRINTF("Triangle Position %d \n", (int)CollisionBuffer[CollisionCount].Geometry.Vertex[ThisVert][ThisVector]);

            if (VertexCache[Index[ThisVert]][ThisVector] < CollisionBuffer[CollisionCount].BoundingMin[ThisVector])
            {
                CollisionBuffer[CollisionCount].BoundingMin[ThisVector] = VertexCache[Index[ThisVert]][ThisVector];
            }

            if (VertexCache[Index[ThisVert]][ThisVector] > CollisionBuffer[CollisionCount].BoundingMax[ThisVector])
            {
                CollisionBuffer[CollisionCount].BoundingMax[ThisVector] = VertexCache[Index[ThisVert]][ThisVector];
            }
        }       
        
    }
    
    SaveNormal();
    SaveAxis();

    CollisionCount++;

    if (CollisionCount >= MAXTRI)
    {
        return;
    }

    Index[0] = (OpCode->CodeB & 0xFF) / 2;
    Index[1] = ((OpCode->CodeB >> 8) & 0xFF) / 2;
    Index[2] = ((OpCode->CodeB >> 16) & 0xFF) / 2;

    

    PRINTF("Triangle # %d \n", (int)CollisionCount);
    PRINTF("Triangle Index0 X %d \n", (int)Index[0]);
    PRINTF("Triangle Index1 Y %d \n", (int)Index[1]);
    PRINTF("Triangle Index2 Z %d \n", (int)Index[2]);

    for (int ThisVector = 0; ThisVector < 3; ThisVector++)
    {
        CollisionBuffer[CollisionCount].BoundingMax[ThisVector] = -99999999999.0f;
        CollisionBuffer[CollisionCount].BoundingMin[ThisVector] = 99999999999.0f;
    }

    for (int ThisVert = 0; ThisVert < 3; ThisVert++)
    {
        PRINTF("Triangle Vertex # %d \n", (int)ThisVert);
        for (int ThisVector = 0; ThisVector < 3; ThisVector++)
        {
            CollisionBuffer[CollisionCount].Geometry.Vertex[ThisVert][ThisVector] = VertexCache[Index[ThisVert]][ThisVector];
            
            PRINTF("Triangle Position %d \n", (int)CollisionBuffer[CollisionCount].Geometry.Vertex[ThisVert][ThisVector]);

            if (VertexCache[Index[ThisVert]][ThisVector] < CollisionBuffer[CollisionCount].BoundingMin[ThisVector])
            {
                CollisionBuffer[CollisionCount].BoundingMin[ThisVector] = VertexCache[Index[ThisVert]][ThisVector];
            }

            if (VertexCache[Index[ThisVert]][ThisVector] > CollisionBuffer[CollisionCount].BoundingMax[ThisVector])
            {
                CollisionBuffer[CollisionCount].BoundingMax[ThisVector] = VertexCache[Index[ThisVert]][ThisVector];
            }
        }       
        
    }

    SaveNormal();
    SaveAxis();
    
    CollisionCount++;

}



void BuildCollisionBuffer(uint Address)
{   
    CollisionCount = 0;
    uint LocalCode;
    while(1)
    {
        CollisionEntry* CEntry = (CollisionEntry*)Address;

        if (CEntry->F3DAddress == 0)
        {
            return;
        }

        uint LocalAddress = (GetRealAddress(CEntry->F3DAddress));

        while(1)
        {
            F3DCode* OpCode = (F3DCode*)LocalAddress;
            LocalCode = OpCode->CodeA & 0xFF000000;
            
            if (LocalCode == ((G_VTX)<<24))
            {
                LoadCache(OpCode);
            }

            if (LocalCode == ((G_TRI1)<<24))
            {
                Save1Triangle(OpCode);
            }

            if (LocalCode == ((G_TRI2)<<24))
            {
                Save2Triangle(OpCode);
            }

            if (LocalCode == ((G_DL)<<24))
            {
                //Save2Triangle(OpCode);
            }

            if (LocalCode == ((G_ENDDL)<<24))
            {
                break;
            }

            
            LocalAddress += 8;

        }

        Address += 8;


    };
}
Vector A, B, C;
float LengthA, LengthB;

void SaveNormal() 
{
    


    Triangle* LocalTriangle = (Triangle*)&CollisionBuffer[CollisionCount].Geometry;
    
    
    A[0] = LocalTriangle->Vertex[1][0] - LocalTriangle->Vertex[0][0];
    A[1] = LocalTriangle->Vertex[1][1] - LocalTriangle->Vertex[0][1];
    A[2] = LocalTriangle->Vertex[1][2] - LocalTriangle->Vertex[0][2];

    B[0] = LocalTriangle->Vertex[2][0] - LocalTriangle->Vertex[0][0];
    B[1] = LocalTriangle->Vertex[2][1] - LocalTriangle->Vertex[0][1];
    B[2] = LocalTriangle->Vertex[2][2] - LocalTriangle->Vertex[0][2];

    // Calculate the cross product of A and B to get the normal
    C[0] = A[1] * B[2] - A[2] * B[1];
    C[1] = A[2] * B[0] - A[0] * B[2];
    C[2] = A[0] * B[1] - A[1] * B[0];


    LengthA = (float)(C[0] * C[0] + C[1] * C[1] + C[2] * C[2]);
    LengthB = (float)sqrtf(LengthA);
    if (LengthB != 0)
    {
        LocalTriangle->Normal[0] = (float)C[0]/LengthB;
        LocalTriangle->Normal[1] = (float)C[1]/LengthB;
        LocalTriangle->Normal[2] = (float)C[2]/LengthB;
    }

    
    
}

void SaveAxis()
{
    CollisionTri* LocalCollide = (CollisionTri*)&CollisionBuffer[CollisionCount];

    A[0] = LocalCollide->Geometry.Normal[0] * LocalCollide->Geometry.Normal[0];
    A[1] = LocalCollide->Geometry.Normal[1] * LocalCollide->Geometry.Normal[1];
    A[2] = LocalCollide->Geometry.Normal[2] * LocalCollide->Geometry.Normal[2];

    if (A[0] > A[1])
    {
        if (A[0] > A[2])
        {
            LocalCollide->NormalVector = XAXISVECTOR;
            return;
        }
    }
    
    if (A[1] > A[0])
    {
        if (A[1] > A[2])
        {
            LocalCollide->NormalVector = YAXISVECTOR;
            return;
        }
    }

    if (A[2] > A[0])
    {
        if (A[2] > A[1])
        {
            LocalCollide->NormalVector = ZAXISVECTOR;
            return;
        }
    }
    //lol wtf

    LocalCollide->NormalVector = WTFAXISVECTOR;
}


int CheckXZ(Sphere Player, ushort ThisTri)
{
    Triangle* Target = (Triangle*)(&CollisionBuffer[ThisTri].Geometry);

    short HitResult;
    //gaiseki1=(p2z-p1z)*(p3x-p1x)-(p2x-p1x)*(p3z-p1z);
    Vector Product;


    Product[0] = 
    (
        (Target->Vertex[0][2] - Player.Center[2]) *
        (Target->Vertex[1][0] - Player.Center[0]) -
        (Target->Vertex[0][0] - Player.Center[0]) *
        (Target->Vertex[1][2] - Player.Center[2]) 
    );
    Product[1] = 
    (
        (Target->Vertex[1][2] - Player.Center[2]) *
        (Target->Vertex[2][0] - Player.Center[0]) -
        (Target->Vertex[1][0] - Player.Center[0]) *
        (Target->Vertex[2][2] - Player.Center[2]) 
    );
    Product[2] = 
    (
        (Target->Vertex[2][2] - Player.Center[2]) *
        (Target->Vertex[0][0] - Player.Center[0]) -
        (Target->Vertex[2][0] - Player.Center[0]) *
        (Target->Vertex[0][2] - Player.Center[2]) 
    );

    //Initial Check

    if (Product[0] == 0)
    {
        //Negative Result
        //Secondary Check Alpha
        
    
        if ((Product[1]*Product[2]) < 0)
        {
            HitResult = 0;
            return 0;
        }
    }
    else
    {
        if (Product[1] == 0)
        {
            if ((Product[0]*Product[2]) < 0)
            {
                HitResult = 0;
                return 0;
            }
        }
        else
        {
            if (Product[0] * Product[1] < 0)
            {
                HitResult = 0;
                return 0;
            }
            else
            {
                if (Product[2] != 0)
                {
                    if (Product[1] * Product[2] < 0)
                    {
                        HitResult = 0;
                        return 0;
                    }
                }
            }
        }
    }

    return 1;
}

int CheckXY(Sphere Player, ushort ThisTri)
{
    Triangle* Target = (Triangle*)(&CollisionBuffer[ThisTri].Geometry);

    short HitResult;
    Vector Product;


    Product[0] = 
    (
        (Target->Vertex[0][1] - Player.Center[1]) *
        (Target->Vertex[1][0] - Player.Center[0]) -
        (Target->Vertex[0][0] - Player.Center[0]) *
        (Target->Vertex[1][1] - Player.Center[1]) 
    );
    Product[1] = 
    (
        (Target->Vertex[1][1] - Player.Center[1]) *
        (Target->Vertex[2][0] - Player.Center[0]) -
        (Target->Vertex[1][0] - Player.Center[0]) *
        (Target->Vertex[2][1] - Player.Center[1]) 
    );
    Product[2] = 
    (
        (Target->Vertex[2][1] - Player.Center[1]) *
        (Target->Vertex[0][0] - Player.Center[0]) -
        (Target->Vertex[2][0] - Player.Center[0]) *
        (Target->Vertex[0][1] - Player.Center[1]) 
    );

    //Initial Check

    if (Product[0] == 0)
    {
        //Negative Result
        //Secondary Check Alpha
        if ((Product[1]*Product[2]) < 0)
        {
            HitResult = 0;
            return 0;
        }
    }
    else
    {
        if (Product[1] == 0)
        {
            if ((Product[0]*Product[2]) < 0)
            {
                HitResult = 0;
                return 0;
            }
        }
        else
        {
            if (Product[0] * Product[1] < 0)
            {
                HitResult = 0;
                return 0;
            }
            else
            {
                if (Product[2] != 0)
                {
                    if (Product[1] * Product[2] < 0)
                    {
                        HitResult = 0;
                        return 0;
                    }
                }
            }
        }
    }

    return 1;
}

int CheckYZ(Sphere Player, ushort ThisTri)
{
    Triangle* Target = (Triangle*)(&CollisionBuffer[ThisTri].Geometry);

    short HitResult;
    
    Vector Product;


    Product[0] = 
    (
        (Target->Vertex[0][1] - Player.Center[1]) *
        (Target->Vertex[1][2] - Player.Center[2]) -
        (Target->Vertex[0][2] - Player.Center[2]) *
        (Target->Vertex[1][1] - Player.Center[1]) 
    );
    Product[1] = 
    (
        (Target->Vertex[1][1] - Player.Center[1]) *
        (Target->Vertex[2][2] - Player.Center[2]) -
        (Target->Vertex[1][2] - Player.Center[2]) *
        (Target->Vertex[2][1] - Player.Center[1]) 
    );
    Product[2] = 
    (
        (Target->Vertex[2][1] - Player.Center[1]) *
        (Target->Vertex[0][2] - Player.Center[2]) -
        (Target->Vertex[2][2] - Player.Center[2]) *
        (Target->Vertex[0][1] - Player.Center[1]) 
    );

    //Initial Check

    if (Product[0] == 0)
    {
        if ((Product[1]*Product[2]) < 0)
        {
            HitResult = 0;
            return 0;
        }
    }
    else
    {
        if (Product[1] == 0)
        {
            if ((Product[0]*Product[2]) < 0)
            {
                HitResult = 0;
                return 0;
            }
        }
        else
        {
            if (Product[0] * Product[1] < 0)
            {
                HitResult = 0;
                return 0;
            }
            else
            {
                if (Product[2] != 0)
                {
                    if (Product[1] * Product[2] < 0)
                    {
                        HitResult = 0;
                        return 0;
                    }
                }
            }
        }
    }

    return 1;
}

void CollisionCheck(int Index)
{
    Sphere CamSphere;
    CamSphere.Center[0] = GameCamera[Index].Position[0];
    CamSphere.Center[1] = GameCamera[Index].Position[1];
    CamSphere.Center[2] = GameCamera[Index].Position[2];
    CamSphere.Radius = 3.0f;

    for (int ThisTri = 0; ThisTri < CollisionCount; ThisTri++)
    {
        for (int ThisVector = 0; ThisVector < 3; ThisVector++)
        {
            if (CamSphere.Center[ThisVector] > CollisionBuffer[ThisTri].BoundingMax[ThisVector])
            {
                goto skiptriangle;
            }
            if (CamSphere.Center[ThisVector] < CollisionBuffer[ThisTri].BoundingMin[ThisVector])
            {
                goto skiptriangle;
            }
        }
        
        int HitResult = -1;
        switch (CollisionBuffer[ThisTri].NormalVector)
        {
            case XAXISVECTOR:
            {
                //HitResult = CheckYZ(CamSphere, ThisTri);
                break;
            }
            case YAXISVECTOR:
            {
                //HitResult = CheckXZ(CamSphere, ThisTri);
                break;
            }
            case ZAXISVECTOR:
            {
                HitResult = CheckXY(CamSphere, ThisTri);
                break;
            }
        }
        if (HitResult > 0)
        {
            GameCamera[Index].Position[0] -= CollisionBuffer[ThisTri].Geometry.Normal[0] * CamSphere.Radius;
            GameCamera[Index].Position[1] -= CollisionBuffer[ThisTri].Geometry.Normal[1] * CamSphere.Radius;
            GameCamera[Index].Position[2] -= CollisionBuffer[ThisTri].Geometry.Normal[2] * CamSphere.Radius;
            PRINTF("Collision %d \n",ThisTri);
            PRINTF("Player X %d \n", (int)GamePlayer[0].Position[0]);
            PRINTF("Player Y %d \n", (int)GamePlayer[0].Position[1]);
            PRINTF("Player Z %d \n", (int)GamePlayer[0].Position[2]);
            for (int ThisVert = 0; ThisVert < 3; ThisVert++)
            {
                PRINTF("Triangle V0 X %d \n", (int)CollisionBuffer[ThisTri].Geometry.Vertex[ThisVert][0]);
                PRINTF("Triangle V0 Y %d \n", (int)CollisionBuffer[ThisTri].Geometry.Vertex[ThisVert][1]);
                PRINTF("Triangle V0 Z %d \n", (int)CollisionBuffer[ThisTri].Geometry.Vertex[ThisVert][2]);
            }
        }

        skiptriangle:
    }
    
}
#endif