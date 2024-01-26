#pragma once
#include <Arduino.h>

struct Size
{
  int Width;
  int Height;
};

struct Vector2
{
  int X;
  int Y;
};


namespace MatrixDisplay
{

  void Clear(bool state = false);

  void Init();

  void Periodic();

  void SetPixel(uint8_t X, uint8_t Y, bool state);

  void SetLine(uint8_t Y, uint8_t value);

  void Set(uint8_t* value);
}
