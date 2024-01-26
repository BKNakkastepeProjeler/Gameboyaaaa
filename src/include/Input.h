#pragma once
#include "Pinout.h"
#include "Display/MatrixDisplay.h"

enum InputButton
{
    LeftBTN  = PIN_BTN_L,
    RightBTN = PIN_BTN_R,
    UpBTN   =  PIN_BTN_U,
    DownBTN =  PIN_BTN_D
};

enum Direction
{
    Left = 0,
    Right,
    Up,
    Down,
    NONE
};

namespace Input
{

    void Init();

    inline bool ReadInpButton(InputButton pin);

    Direction GetDirection(bool debounce = true);

    Point GetDirectionVector(bool debounce = true);
    Point GetDirectionVector(Direction dir);
}