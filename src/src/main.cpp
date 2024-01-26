#include <Arduino.h>
#include "Display/MatrixDisplay.h"
#include "Input.h"
#include "Game/GameHandler.h"

void setup()
{
    Serial.begin(9600);
    Input::Init();
    MatrixDisplay::Init(); 
    GameHandler::Init();
}

void loop()
{
    GameHandler::Periodic();
    MatrixDisplay::Periodic();
}