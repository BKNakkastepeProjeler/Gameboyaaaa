#pragma once

#include "Game/BaseGame.h"

class DodgeGame : public BaseGame
{
public:
    uint8_t Icon[8] =
        {
            0b10000001,
            0b10000001,
            0b10000001,
            0b10000001,
            0b10000001,
            0b10000001,
            0b10000001,
            0b10000001
        };

    uint8_t *GetIcon()
    {
        return Icon;
    }
    //-----------------------
    struct Obstacle
    {
        int Y;
        int Width;
        bool AlignToLeft;
    };
    
    //--------------------------------------------------

    const Size GridSize = MatrixDisplay::GridSize;

    const int obstacleYGap = 4;

    const int obstacleSpeed = 400;

    Point player;

    Obstacle obstacles[3];
    bool obstacleActive[3] = {false, false, false};

    //--------------------------------------------------

    void EndAnimation(WinState state)
    {
        
    }

    inline int GetEmptyObstacleSlot()
    {
        for (int i = 0; i < 3; i++)
        {
            if (!obstacleActive[i])
                return i;
        }

        return -1;
    }
    

    void CreateObstacle()
    {
        for(int i = 0; i<3; i++)
        {
            if(!obstacleActive[i]) continue;
            Obstacle obs = obstacles[i];

            if(obs.Y < obstacleYGap + 1) return;
        }

        int slot = GetEmptyObstacleSlot();
        if(slot == -1) return;

        Obstacle newObstacle = {0, random(4, MatrixDisplay::GridSize.Width - 1), random(0, 2) == 0};
        Serial.println("New Obstacle");
        obstacles[slot] = newObstacle;
        obstacleActive[slot] = true;
    }

    void CheckCollide()
    {
        for(int i = 0; i<3; i++)
        {
            if(!obstacleActive[i]) continue;
            Obstacle obs = obstacles[i];

            if(obs.Y != player.Y) continue;

            if(obs.AlignToLeft)
            {
                if ((obs.AlignToLeft && player.X < obs.Width) || (!obs.AlignToLeft && player.X > GridSize.Width - obs.Width))
                {
                    GameHandler::EndGame(WinState::Lost);
                    return;
                }
                
            }
        }
    }

    AsyncDelay ObstacleMoveDelay;

    void MoveObstacles()
    {
        if(!ObstacleMoveDelay.isExpired()) return;
        ObstacleMoveDelay.start(obstacleSpeed, AsyncDelay::MILLIS);
        for(int i = 0; i<3; i++)
        {
            if(!obstacleActive[i]) continue;
            Obstacle* obs = &obstacles[i];
            
            obs->Y += 1;
            if(obs->Y >= GridSize.Height)
            {
                obstacleActive[i] = false;
            }
        }
    }

    void Draw()
    {
        MatrixDisplay::Clear();

        MatrixDisplay::SetPixel(player.X, player.Y, true);

        for(int i = 0; i<3; i++)
        {
            if(!obstacleActive[i]) continue;
            Obstacle obs = obstacles[i];

            for(int x = 0; x<obs.Width; x++)
            {
                int xCoord = obs.AlignToLeft ? x : GridSize.Width - x - 1;
                MatrixDisplay::SetPixel(xCoord, obs.Y, true);
            }
        
        }
    }
    
    void MovePlayer()
    {
        Point vector = Input::GetDirectionVector();
        
        int newPlayerX = player.X + vector.X;
        if(newPlayerX < 0 || newPlayerX >= GridSize.Width) return;

        player.X = newPlayerX;
    }


    void Begin()
    {
        player = {5, GridSize.Height - 2};

        for(int i = 0; i<3; i++)
        {
            obstacleActive[i] = false;
        }

        ObstacleMoveDelay.start(obstacleSpeed, AsyncDelay::MILLIS);
    }

    void Periodic()
    {
        MovePlayer();

        CheckCollide();

        CreateObstacle();

        MoveObstacles();

        Draw();
    }
};