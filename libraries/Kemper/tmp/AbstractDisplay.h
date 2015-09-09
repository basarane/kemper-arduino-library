
#pragma once

#include "Utils.h"

USING_NAMESPACE_KEMPER

class AbstractDisplay
{
private:
	char textHolder[128];

public:
	int width;
	int height;
	virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) = 0;
	virtual void drawText(int16_t x, int16_t y, int16_t size, const char* text, int16_t len, uint16_t color, int16_t rotation) = 0;
	void drawText(int16_t x, int16_t y, int16_t size, const char* str, int16_t len, uint16_t color, int16_t rotation, int letterCount);
};

