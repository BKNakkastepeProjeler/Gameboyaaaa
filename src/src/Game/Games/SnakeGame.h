#pragma once

#include "Game/BaseGame.h"

class SnakeGame : public BaseGame
{
public:
    uint8_t Icon[8] =
        {
            (uint8_t)0b11111100,
            (uint8_t)0b11111100,
            (uint8_t)0b00001100,
            (uint8_t)0b00001100,
            (uint8_t)0b00001100,
            (uint8_t)0b00000000,
            (uint8_t)0b00001100,
            (uint8_t)0b00001100};

    uint8_t *GetIcon()
    {
        return Icon;
    }

    //--------------------------------------------------

    const Size GridSize = MatrixDisplay::GridSize;

    const int speed = 300;

    int snakeCellCount;

    Point Snake[64];

    Point FoodPoint;

    Direction direction = Direction::Right;

    AsyncDelay mainDelay;

    //--------------------------------------------------

    

    void EndAnimation(WinState state)
    {
        AsyncDelay animDelay = AsyncDelay(100, AsyncDelay::MILLIS);

        for(int i = 0; i<snakeCellCount; i++)
        {
            Point p = Snake[i];
            MatrixDisplay::SetPixel(p.X, p.Y, false);
            animDelay.start(100, AsyncDelay::MILLIS);

            while(!animDelay.isExpired())
            {
                MatrixDisplay::Periodic();
            }

            animDelay.restart();
        }
        animDelay.start(800, AsyncDelay::MILLIS);
        while(!animDelay.isExpired())
        {
            MatrixDisplay::Periodic();
        }
    }

    void Draw()
    {
        MatrixDisplay::Clear();

        for (int i = 0; i < snakeCellCount; i++)
        {
            Point p = Snake[i];
            MatrixDisplay::SetPixel(p.X, p.Y, true);
        }

        MatrixDisplay::SetPixel(FoodPoint.X, FoodPoint.Y, true);
    }
        
    void createFood()
    {
        Point food = {random(0, GridSize.Width), random(0, GridSize.Height)};

        for (int i = 0; i < snakeCellCount; i++)
        {
            Point snakeCell = Snake[i];

            if (snakeCell.X == food.X && snakeCell.Y == food.Y)
            {
                createFood();
                return;
            }
        }

        FoodPoint = food;
    }

    inline void MainLogic()
    {
        Point offset = Input::GetDirectionVector(direction);
        Point head = Snake[0];
        Point newHead = {head.X + offset.X, head.Y + offset.Y};

        if (!MatrixDisplay::IsInBounds(newHead))
        {
            GameHandler::EndGame(WinState::Lost);
            return;
        }

        Point lastCellPoint;

        for (int i = snakeCellCount - 1; i > 0; i--)
        {
            Snake[i] = Snake[i - 1];

            if (i == snakeCellCount - 1)
                lastCellPoint = Snake[i];
        }

        Snake[0] = newHead;

        if (newHead.X == FoodPoint.X && newHead.Y == FoodPoint.Y)
        {
            Snake[snakeCellCount] = lastCellPoint;
            snakeCellCount++;
            createFood();
        }

        Draw();
    }
    

    void Begin()
    {
        snakeCellCount = 2;
        Snake[0] = {1, 0};
        Snake[1] = {0, 0};
        direction = Direction::Right;

        createFood();

        Draw();

        mainDelay.start(speed, AsyncDelay::MILLIS);
    }

    void Periodic()
    {
        Direction drc = Input::GetDirection();
        if (drc != Direction::NONE)
            direction = drc;

        if (!mainDelay.isExpired())
            return;

        MainLogic();

        mainDelay.start(speed, AsyncDelay::MILLIS);
    }
};