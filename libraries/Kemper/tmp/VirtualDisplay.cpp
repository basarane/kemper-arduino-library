
#include "VirtualDisplaySerializer.h"
#include "VirtualDisplay.h"
#include <math.h>


VirtualDisplay::VirtualDisplay(AbstractDisplaySerializer* _Serial, int _width, int _height) {
	Serial = _Serial;
	width = _width;
	height = _height;
}

void VirtualDisplay::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  rectHelper(x, y, x+w, y+h, color, false);
}

void VirtualDisplay::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  rectHelper(x, y, x+w, y+h, color, true);
}

void VirtualDisplay::rectHelper(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, bool filled)
{
  Serial->start();
  Serial->transfer((int16_t)1);
  Serial->transfer(x);
  Serial->transfer(y);
  Serial->transfer(w);
  Serial->transfer(h);
  Serial->transfer(color);
  Serial->transfer(filled?1:0);
  Serial->end();
}

void VirtualDisplay::drawText(int16_t x, int16_t y, int16_t size, const char* text, int16_t len, uint16_t color, int16_t rotation)
{
  Serial->start();
  Serial->transfer((int16_t)2);
  Serial->transfer(x);
  Serial->transfer(y);
  Serial->transfer(size);
  Serial->transfer(color);
  Serial->transfer(rotation);
  Serial->transfer(len);
  for (int i=0;i<len;i++)
	Serial->transfer(text[i]);
  Serial->end();
}
