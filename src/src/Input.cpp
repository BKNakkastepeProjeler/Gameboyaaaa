#include "Pinout.h"
#include "Input.h"
#include "Display/MatrixDisplay.h"

namespace Input
{

    inline void PullUpInit(InputButton pin)
    {
        pinMode(pin, INPUT_PULLUP);
    }

    void Init()
    {
        PullUpInit(InputButton::LeftBTN);
        PullUpInit(InputButton::RightBTN);
        PullUpInit(InputButton::UpBTN);
        PullUpInit(InputButton::DownBTN);
    }

    inline bool ReadInpButton(InputButton pin)
    {
        return digitalRead(pin) == LOW;
    }

    bool leftBtnLastState = false;
    bool rightBtnLastState = false;
    bool upBtnLastState = false;
    bool downBtnLastState = false;
    

    bool CheckInpButtonWithDebounce(InputButton pin, bool& lastState)
    {
        bool state = ReadInpButton(pin);

        if(state != lastState)
        {
            lastState = state;
            if(state) return true;
        }

        return false;
    }

    Direction GetDirection(bool debounce)
    {
        if (debounce)
        {
            if(CheckInpButtonWithDebounce(InputButton::LeftBTN, leftBtnLastState)) return Direction::Left;
            if(CheckInpButtonWithDebounce(InputButton::RightBTN, rightBtnLastState)) return Direction::Right;
            if(CheckInpButtonWithDebounce(InputButton::UpBTN, upBtnLastState)) return Direction::Up;
            if(CheckInpButtonWithDebounce(InputButton::DownBTN, downBtnLastState)) return Direction::Down;

            return Direction::NONE;
        }
        else
        {
            if (ReadInpButton(InputButton::LeftBTN))
                return Direction::Left;
            if (ReadInpButton(InputButton::RightBTN))
                return Direction::Right;

            if (ReadInpButton(InputButton::UpBTN))
                return Direction::Up;
            if (ReadInpButton(InputButton::DownBTN))
                return Direction::Down;

            return Direction::NONE;
        }
    }

    Point GetDirectionVector(Direction dir)
    {
        switch (dir)
        {

        default:
        case Direction::NONE:
            return {0, 0};

        case Direction::Left:
            return {-1, 0};
            
        case Direction::Right:
            return {1, 0};

        case Direction::Up:
            return {0, -1};

        case Direction::Down:
            return {0, 1};

        }
    }

    Point GetDirectionVector(bool debounce)
    {
        Direction dir = GetDirection(debounce);

        return GetDirectionVector(dir);
    }


}