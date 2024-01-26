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
            (uint8_t)0b00001100
        };

        uint8_t* GetIcon()
        {
            return Icon;
        }


        //--------------------------------------------------


        Vector2 pos = {0,0};
        Vector2 oldPos = {0,0};

        void Draw()
        {
            MatrixDisplay::Clear();
            MatrixDisplay::SetPixel(pos.X, pos.Y, true);
        }

        void Begin()
        {

        }

        void Periodic()
        {
            Vector2 vec = Input::GetDirectionVector();
            pos.X += vec.X;
            pos.Y += vec.Y;
            if(pos.X != oldPos.X || pos.Y != oldPos.Y)
            {
                Serial.printf("X: %d, Y: %d\n", pos.X, pos.Y);
                oldPos.X = pos.X;
                oldPos.Y = pos.Y;
            }
            Draw();
        }
};