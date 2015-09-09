
#include "VirtualDisplaySerializer.h"
#include "VirtualDisplay.h"
#include <math.h>


VirtualDisplay::VirtualDisplay(AbstractDisplaySerializer* _Serial, int _width, int _height) {
	Serializer = _Serial;
	width = _width;
	height = _height;
}

void VirtualDisplay::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  rectHelper(x, y, x+w, y+h, 0, color, false);
}

void VirtualDisplay::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  rectHelper(x, y, x+w, y+h, 0, color, true);
}
void VirtualDisplay::fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t radius, uint16_t color) {
  rectHelper(x, y, x+w, y+h, radius, color, true);
}

void VirtualDisplay::rectHelper(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t radius, uint16_t color, bool filled)
{
	//return;
  Serializer->start();
  Serializer->transfer((int16_t)1);
  Serializer->transfer(x);
  Serializer->transfer(y);
  Serializer->transfer(w);
  Serializer->transfer(h);
  Serializer->transfer(radius);
  Serializer->transfer(color);
  Serializer->transfer(filled?1:0);
  Serializer->end();
}

void VirtualDisplay::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
  Serializer->start();
  Serializer->transfer((int16_t)3);
  Serializer->transfer(x0);
  Serializer->transfer(y0);
  Serializer->transfer(x1);
  Serializer->transfer(y1);
  Serializer->transfer(x2);
  Serializer->transfer(y2);
  Serializer->transfer(color);
  Serializer->end();
}


/*
void VirtualDisplay::drawText(int16_t x, int16_t y, int16_t size, const char* text, int16_t len, uint16_t color, int16_t rotation)
{
//return;
  Serializer->start();
  Serializer->transfer((int16_t)2);
  Serializer->transfer(x);
  Serializer->transfer(y);
  Serializer->transfer(size);
  Serializer->transfer(color);
  Serializer->transfer(rotation);
  Serializer->transfer(len);
  for (int i=0;i<len;i++)
	Serializer->transfer(text[i]);
  Serializer->end();
  //Serial->transferString(text);
}
*/
void VirtualDisplay::drawText(int16_t x, int16_t y, int16_t w, int16_t h, TextAlignHorizontal horAlign, TextAlignVertical verAlign, int16_t size, const char* text, int16_t len, uint16_t color) {
  Serializer->start();
  Serializer->transfer((int16_t)2);
  Serializer->transfer(x);
  Serializer->transfer(y);
  Serializer->transfer(w);
  Serializer->transfer(h);
  Serializer->transfer((uint16_t)horAlign);
  Serializer->transfer((uint16_t)verAlign);
  Serializer->transfer(size);
  Serializer->transfer(color);
  Serializer->transfer(len);
  for (int i=0;i<len;i++)
	Serializer->transfer(text[i]);
  Serializer->end();
}