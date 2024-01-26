#pragma once

#include <Arduino.h>
#include "Input.h"
#include "Display/MatrixDisplay.h"

class BaseGame
{
    public:

        virtual uint8_t* GetIcon() = 0;

        virtual void Begin() = 0;

        virtual void Periodic() = 0;
};