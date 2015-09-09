
#ifndef _ABSTRACT_DISPLAY_H
#define _ABSTRACT_DISPLAY_H

#include "Utils.h"

USING_NAMESPACE_KEMPER

enum TextAlignVertical {TextAlignTop, TextAlignMiddle, TextAlignBottom};
enum TextAlignHorizontal {TextAlignLeft, TextAlignCenter, TextAlignRight};

class AbstractDisplay
{
private:
	char textHolder[128];

public:
	int width;
	int height;
	bool supportsLayers;
	virtual void writeTo(int layer) {};
	virtual void setTransparentColor(uint16_t color) {};
	virtual void layerEffect(int layerEffect) {};
	virtual void clearRect(int16_t x, int16_t y, int16_t w, int16_t h) {};
	virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) = 0;
	virtual void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) = 0;
	virtual void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t radius, uint16_t color) = 0;
	virtual void drawText(int16_t x, int16_t y, int16_t w, int16_t h, TextAlignHorizontal horAlign, TextAlignVertical verAlign, int16_t size, const char* text, int16_t len, uint16_t color) = 0;
	//virtual void drawText(int16_t x, int16_t y, int16_t size, const char* text, int16_t len, uint16_t color, int16_t rotation) = 0;
	//void drawText(int16_t x, int16_t y, int16_t size, const char* str, int16_t len, uint16_t color, int16_t rotation, int letterCount);
};

#endif