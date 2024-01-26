#include "Game/BaseGame.h"

class TurretGame : public BaseGame
{
public:
    uint8_t Icon[8] =
        {
            0b00010000,
            0b10000010,
            0b00010000,
            0b01000100,
            0b00010000,
            0b10000001,
            0b00010000,
            0b00111000
        };

    const Size GridSize = MatrixDisplay::GridSize;

    const int bulletSpeed = 100;
    const int meteorSpeed = 500;
    const int meteorSpawnSpeed = 1000;

    Point meteors[3];
    bool meteorFired[3];

    Point bullets[5];
    AsyncDelay bulletDelays[5];
    bool bulletFired[5];

    Point gunPoint = {3, 6};

    uint8_t *GetIcon()
    {
        return Icon;
    }

    void EndAnimation(WinState state)
    {
        if(state != WinState::Lost) return;

        uint8_t anim[10][8] = 
        {
            {
                0b00000000,
                0b00000000,
                0b00000000,
                0b00000000,
                0b00000000,
                0b00000000,
                0b00000000,
                0b00011000
            },
            {
                0b00000000,
                0b00000000,
                0b00000000,
                0b00000000,
                0b00000000,
                0b00000000,
                0b00011000,
                0b00111100
            },
            {
                0b00000000,
                0b00000000,
                0b00000000,
                0b00000000,
                0b00000000,
                0b00011000,
                0b00111100,
                0b01111110
            },
            {
                0b00000000,
                0b00000000,
                0b00000000,
                0b00011000,
                0b00111100,
                0b01111110,
                0b11111111,
                0b11100111
            },
            {
                0b00000000,
                0b00011000,
                0b00111100,
                0b01111110,
                0b11111111,
                0b11111111,
                0b11111111,
                0b11000011
            },
            {
                0b00111100,
                0b01111110,
                0b11111111,
                0b11111111,
                0b11111111,
                0b11100111,
                0b11000011,
                0b10000001
            },
            {
                0b11111111,
                0b11111111,
                0b11111111,
                0b11100111,
                0b11000011,
                0b10000001,
                0b00000000,
                0b00000000
            },
            {
                0b11111111,
                0b11100111,
                0b11000011,
                0b10000001,
                0b00000000,
                0b00000000,
                0b00000000,
                0b00000000
            },
            {
                0b11000011,
                0b10000001,
                0b00000000,
                0b00000000,
                0b00000000,
                0b00000000,
                0b00000000,
                0b00000000
            },
            {
                0b00000000,
                0b00000000,
                0b00000000,
                0b00000000,
                0b00000000,
                0b00000000,
                0b00000000,
                0b00000000
            }
        };
        
        for (int i = 0; i < 10; i++)
        {
            MatrixDisplay::Set(anim[i]);
            MatrixDisplay::PeriodicFor(100);
        }
        

    }

    void Draw()
    {
        MatrixDisplay::Clear();

        // Gun

        MatrixDisplay::SetPixel(gunPoint.X, gunPoint.Y, true);
        MatrixDisplay::SetPixel(gunPoint.X, gunPoint.Y + 1, true);
        MatrixDisplay::SetPixel(gunPoint.X - 1, gunPoint.Y + 1, true);
        MatrixDisplay::SetPixel(gunPoint.X + 1, gunPoint.Y + 1, true);

        // Bullets

        for (int i = 0; i < 5; i++)
        {
            if (!bulletFired[i])
                continue;
            Point bullet = bullets[i];
            MatrixDisplay::SetPixel(bullet.X, bullet.Y, true);
        }

        // Meteors

        for (int i = 0; i < 3; i++)
        {
            if (!meteorFired[i])
                continue;
            Point meteor = meteors[i];
            MatrixDisplay::SetPixel(meteor.X, meteor.Y, true);
        }
    }

    inline int GetEmptyBulletSlot()
    {
        for (int i = 0; i < 5; i++)
        {
            if (!bulletFired[i])
                return i;
        }

        return -1;
    }

    inline void CreateBullet()
    {
        int emptySlot = GetEmptyBulletSlot();
        if (emptySlot == -1)
            return;

        bulletFired[emptySlot] = true;
        bullets[emptySlot] = {gunPoint.X, gunPoint.Y - 1};
        bulletDelays[emptySlot].start(bulletSpeed, AsyncDelay::MILLIS);
    }

    inline void MoveBullets()
    {
        for (int i = 0; i < 5; i++)
        {
            if (!bulletFired[i])
                continue;
            if (!bulletDelays[i].isExpired())
                continue;
            bulletDelays[i].start(bulletSpeed, AsyncDelay::MILLIS);
            bullets[i].Y--;
            if (bullets[i].Y < 0)
            {
                bulletFired[i] = false;
            }
        }
    }

    inline int GetEmptyMeteorSlot()
    {
        for (int i = 0; i < 3; i++)
        {
            if (!meteorFired[i])
                return i;
        }

        return -1;
    }

    AsyncDelay meteorSpawnDelay;
    inline void CreateMeteor()
    {
        if (!meteorSpawnDelay.isExpired())
            return;
        meteorSpawnDelay.start(meteorSpawnSpeed, AsyncDelay::MILLIS);

        int emptySlot = GetEmptyMeteorSlot();
        if (emptySlot == -1)
            return;

        meteorFired[emptySlot] = true;
        meteors[emptySlot] = {(int)random(0, 8), 0};
    }

    AsyncDelay meteorMoveDelay;

    inline void MoveMeteors()
    {
        if (!meteorMoveDelay.isExpired())
            return;
        meteorMoveDelay.start(meteorSpeed, AsyncDelay::MILLIS);

        for (int i = 0; i < 3; i++)
        {
            if (!meteorFired[i])
                continue;

            meteors[i].Y++;

            if (meteors[i].Y > GridSize.Height - 1)
            {
                meteorFired[i] = false;

                GameHandler::EndGame(WinState::Lost);
            }
        }
    }

    inline void CheckCollision()
    {
        for (int i = 0; i < 3; i++)
        {
            if (!meteorFired[i])
                continue;

            for (int j = 0; j < 5; j++)
            {
                if (!bulletFired[j])
                    continue;

                if (meteors[i].X == bullets[j].X && meteors[i].Y == bullets[j].Y)
                {
                    meteorFired[i] = false;
                    bulletFired[j] = false;
                }
            }
        }
    }
    
    void Update()
    {

        Direction drc = Input::GetDirection();
        
        switch (drc)
        {
        case Direction::NONE: return;
        case Direction::Left:
            if (gunPoint.X > 0)
            {
                gunPoint.X--;
            }
            break;

        case Direction::Right:
            if (gunPoint.X < GridSize.Width - 1)
            {
                gunPoint.X++;
            }
            break;

        case Direction::Up:
            CreateBullet();
            break;

        default: return;
        }
    }

    void Begin()
    {
        for (int i = 0; i < 3; i++)
        {
            meteorFired[i] = false;
        }

        for (int i = 0; i < 5; i++)
        {
            bulletFired[i] = false;
        }

        Draw();
    }

    void Periodic()
    {
        MoveBullets();

        CheckCollision();

        MoveMeteors();

        CreateMeteor();

        Update();

        Draw();
    }
};