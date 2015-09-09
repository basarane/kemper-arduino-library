
#include "AbstractDisplay.h"

USING_NAMESPACE_KEMPER

void AbstractDisplay::drawText(int16_t x, int16_t y, int16_t size, const char* str, int16_t len, uint16_t color, int16_t rotation, int letterCount)
{
	int t = 0;
	char* text = textHolder;
	strcpy(text, str);
	int lastSpace = -1;
	int i=0;
	int line = 0;
	int lineHeight = size;
	char* textp = text;
	while (i<len) {
		if (textp[i] == ' ') 
		{
			lastSpace = i;
		}
		if (t>letterCount && lastSpace>=0) {
			textp[lastSpace] = 0;
			drawText(x -sin(PI*rotation / 18000.0)* line*lineHeight, y + cos(PI*rotation / 18000.0)* line*lineHeight, size, text, strlen(text), color, rotation);
			line++;
			text = textp + lastSpace+1;
			t = i-lastSpace;
			lastSpace = -1;
		}
		t++;
		i++;
	}
	drawText(x-sin(PI*rotation / 18000.0)* line*lineHeight, y + cos(PI*rotation / 18000.0)* line*lineHeight , size, text, strlen(text), color, rotation);
}
