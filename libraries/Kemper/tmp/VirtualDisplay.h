#pragma once

#include "AbstractDisplay.h"
#include "VirtualDisplaySerializer.h"


class VirtualDisplay : public AbstractDisplay {
 public:
  VirtualDisplay(AbstractDisplaySerializer* _Serial, int _width, int _height);
  AbstractDisplaySerializer* Serial;
  
  void    drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
  void    fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
  
  void    drawText(int16_t x, int16_t y, int16_t size, const char* text, int16_t len, uint16_t color, int16_t rotation);
  void    drawText(int16_t x, int16_t y, int16_t size, const char* str, int16_t len, uint16_t color, int16_t rotation, int letterCount);

  void rectHelper  (int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, bool filled);
};
