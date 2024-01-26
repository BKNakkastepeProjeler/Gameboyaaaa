#pragma once

enum WinState
{
    Lost = 0,
    Won,
    Tie
};

#include <Arduino.h>
#include "Input.h"
#include "Display/MatrixDisplay.h"
#include "AsyncDelay.h"
#include "Game/GameHandler.h"



class BaseGame
{
    public:

        virtual uint8_t* GetIcon() = 0;

        virtual void Begin() = 0;

        virtual void Periodic() = 0;
        
        virtual void EndAnimation(WinState state) = 0;
};