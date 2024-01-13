
#include <ultra64.h>
#include "gfx.h"
#include "controller.h"
#include "common.h"
#include "simple.h"
#include "math.h"

Player GamePlayers[4];

float UpDirection[] = { 0.0f, 0.0f, 1.0f};
float LookDirection[] = { 0.0f, 1.0f, 0.0f};


void initPlayer(int ThisPlayer)
{
    for (int ThisVector = 0; ThisVector < 3; ThisVector++)
    {
        GamePlayers[ThisPlayer].Location.Position[ThisVector] = 0.0f;
        GamePlayers[ThisPlayer].Location.Angle[ThisVector] = 0;
        GamePlayers[ThisPlayer].Location.VelocityFront[ThisVector] = 0.0f;
        GamePlayers[ThisPlayer].Location.VelocitySide[ThisVector] = 0.0f;

        GamePlayers[ThisPlayer].Camera.Location.Position[ThisVector] = 0.0f;
        GamePlayers[ThisPlayer].Camera.Location.Angle[ThisVector] = 0;
        GamePlayers[ThisPlayer].Camera.Location.VelocityFront[ThisVector] = 0.0f;
        GamePlayers[ThisPlayer].Camera.Location.VelocitySide[ThisVector] = 0.0f;

        GamePlayers[ThisPlayer].Camera.LookAt[ThisVector] = LookDirection[ThisVector];
        GamePlayers[ThisPlayer].Camera.UpVector[ThisVector] = UpDirection[ThisVector];
        
    }
    
    GamePlayers[ThisPlayer].Camera.Location.Position[1] = -300.0f;
    GamePlayers[ThisPlayer].Location.Position[1] = 300.0f;

    GamePlayers[ThisPlayer].Location.Radius = 4.0f;        
    GamePlayers[ThisPlayer].Camera.Location.Radius = 4.0f;

    GamePlayers[ThisPlayer].Camera.FOVX = 90;
    GamePlayers[ThisPlayer].Camera.FOVY = 60;

    GamePlayers[ThisPlayer].Camera.Near = 2.0f;
    GamePlayers[ThisPlayer].Camera.Far = 16000.0f;
}

void initAllPlayers()
{
    for (int ThisPlayer = 0; ThisPlayer < 4; ThisPlayer++)
    {
        initPlayer(ThisPlayer);
    }
}

AffineMtx CameraMatrix[4];
float Nerf;

#define MAXSPEED 3.5f
void UpdatePlayerControls()
{
    for (int ThisPlayer = 0; ThisPlayer < 4; ThisPlayer++)
    {
        OSContPad* LocalPad = (OSContPad*)&ControllerArray[ThisPlayer];
        PGCamera* LocalCamera = (PGCamera*)&GamePlayers[ThisPlayer].Camera;
        Player* LocalPlayer = (Player*)&GamePlayers[ThisPlayer];
        
        float XSpeed = 0.0f;
        float YSpeed = 0.0f;


        if ((LocalPad->stick_x > 5) || (LocalPad->stick_x < -5))
        {
            XSpeed = (float)LocalPad->stick_x * 0.1f;
        }
        if ((LocalPad->stick_y > 5) || (LocalPad->stick_y < -5))
        {
            YSpeed = (float)LocalPad->stick_y * 0.1f;
        }
        TransformMatrix(CameraMatrix[ThisPlayer],LocalCamera->Location.Position, LocalCamera->LookAt, LocalCamera->UpVector);


        LocalCamera->Location.VelocityFront[0] += CameraMatrix[ThisPlayer][0][2] * YSpeed;
        LocalCamera->Location.VelocityFront[1] += CameraMatrix[ThisPlayer][1][2] * YSpeed;
        LocalCamera->Location.VelocityFront[2] += CameraMatrix[ThisPlayer][2][2] * YSpeed;

        LocalCamera->Location.VelocitySide[0] += CameraMatrix[ThisPlayer][0][0] * XSpeed;
        LocalCamera->Location.VelocitySide[1] += CameraMatrix[ThisPlayer][1][0] * XSpeed;
        LocalCamera->Location.VelocitySide[2] += CameraMatrix[ThisPlayer][2][0] * XSpeed;

        float VelocityA = (
            LocalCamera->Location.VelocityFront[0] * LocalCamera->Location.VelocityFront[0] + 
            LocalCamera->Location.VelocityFront[1] * LocalCamera->Location.VelocityFront[1] + 
            LocalCamera->Location.VelocityFront[2] * LocalCamera->Location.VelocityFront[2]
        );

        float VelocityB = (
            LocalCamera->Location.VelocitySide[0] * LocalCamera->Location.VelocitySide[0] + 
            LocalCamera->Location.VelocitySide[1] * LocalCamera->Location.VelocitySide[1] + 
            LocalCamera->Location.VelocitySide[2] * LocalCamera->Location.VelocitySide[2]
        );
        
        if (VelocityA > MAXSPEED * MAXSPEED)
        {
            Nerf = MAXSPEED * MAXSPEED / VelocityA;
            LocalCamera->Location.VelocityFront[0] *= Nerf;
            LocalCamera->Location.VelocityFront[1] *= Nerf;
            LocalCamera->Location.VelocityFront[2] *= Nerf;
        }

        if (VelocityB > MAXSPEED * MAXSPEED)
        {
            Nerf = MAXSPEED * MAXSPEED / VelocityB;
            LocalCamera->Location.VelocitySide[0] *= Nerf;
            LocalCamera->Location.VelocitySide[1] *= Nerf;
            LocalCamera->Location.VelocitySide[2] *= Nerf;
        }

        for (int ThisVector = 0; ThisVector < 3; ThisVector++)
        {
            LocalCamera->Location.Position[ThisVector] -= LocalCamera->Location.VelocityFront[ThisVector];
            LocalCamera->Location.Position[ThisVector] -= LocalCamera->Location.VelocitySide[ThisVector];

            LocalCamera->LookAt[ThisVector] -= LocalCamera->Location.VelocityFront[ThisVector];
            LocalCamera->LookAt[ThisVector] -= LocalCamera->Location.VelocitySide[ThisVector];

            LocalPlayer->Location.Position[ThisVector] -= LocalCamera->Location.VelocityFront[ThisVector];
            LocalPlayer->Location.Position[ThisVector] -= LocalCamera->Location.VelocitySide[ThisVector];
        }



    }
}
