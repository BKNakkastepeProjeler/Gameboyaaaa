#pragma once
#include <Arduino.h>

struct Size
{
  int Width;
  int Height;
};

struct Point
{
  int X;
  int Y;
};


namespace MatrixDisplay
{
  const Size GridSize = {8, 8};

  void Clear(bool state = false);

  void Init();

  void Periodic();

  void PeriodicFor(int ms);
  
  void SetPixel(uint8_t X, uint8_t Y, bool state);

  void SetLine(uint8_t Y, uint8_t value);

  void Set(uint8_t* value);

  bool IsInBounds(Point p);
}
