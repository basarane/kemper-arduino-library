#pragma once

#include "AbstractDisplay.h"
#include "VirtualDisplaySerializer.h"


class VirtualDisplay : public AbstractDisplay {
 public:
  VirtualDisplay(AbstractDisplaySerializer* _Serial, int _width, int _height);
  AbstractDisplaySerializer* Serializer;
  
  void    drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
  void    fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
  void    fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t radius, uint16_t color);
  void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);

  //void    drawText(int16_t x, int16_t y, int16_t size, const char* text, int16_t len, uint16_t color, int16_t rotation);
  //void    drawText(int16_t x, int16_t y, int16_t size, const char* str, int16_t len, uint16_t color, int16_t rotation, int letterCount);
  void drawText(int16_t x, int16_t y, int16_t w, int16_t h, TextAlignHorizontal horAlign, TextAlignVertical verAlign, int16_t size, const char* text, int16_t len, uint16_t color);

  void rectHelper  (int16_t x, int16_t y, int16_t w, int16_t h, uint16_t radius, uint16_t color, bool filled);
};
