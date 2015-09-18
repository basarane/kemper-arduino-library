
#include "Display_ER_RA8875.h"
#include <math.h>

byte textWidths[][94] = {
	{ 4, 8, 9, 9, 16, 12, 4, 5, 5, 6, 9, 4, 5, 4, 4, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 6, 6, 9, 9, 9, 10, 16, 11, 12, 12, 12, 11, 10, 12, 12, 4, 9, 12, 10, 13, 12, 12, 11, 12, 12, 11, 10, 12, 11, 15, 11, 10, 9, 5, 4, 5, 9, 9, 5, 9, 10, 9, 10, 9, 5, 10, 10, 4, 4, 9, 4, 14, 10, 10, 10, 10, 6, 9, 5, 10, 9, 13, 9, 9, 9, 6, 4, 6, 9 },
	{ 7, 11, 13, 13, 18, 17, 6, 8, 8, 9, 14, 7, 8, 7, 7, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 7, 7, 14, 14, 14, 15, 23, 17, 17, 17, 17, 16, 15, 19, 17, 7, 13, 17, 15, 21, 17, 18, 16, 18, 17, 16, 15, 17, 16, 23, 16, 15, 14, 8, 7, 8, 14, 13, 8, 13, 15, 13, 15, 13, 8, 15, 15, 7, 7, 13, 7, 21, 15, 15, 15, 15, 9, 13, 8, 15, 13, 19, 13, 13, 12, 9, 7, 9, 14 },
	{ 10, 15, 18, 18, 26, 23, 8, 11, 11, 12, 19, 9, 11, 9, 9, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 10, 10, 19, 19, 19, 20, 31, 23, 22, 23, 23, 21, 20, 25, 22, 8, 17, 23, 20, 27, 22, 24, 20, 24, 22, 21, 20, 22, 21, 31, 21, 20, 20, 11, 9, 11, 19, 18, 11, 18, 19, 18, 19, 18, 11, 19, 19, 10, 10, 18, 10, 28, 19, 19, 19, 19, 13, 18, 11, 19, 17, 25, 18, 17, 16, 12, 9, 12, 19 }
};

#define TEXT_WIDTH(b) (((b)>=0x21 && (b)<=0x7e) ? textWidths[(fontSize)][(b) - 0x21]:(size))
//#define TEXT_WIDTH(b) (size)

Display_ER_RA8875::Display_ER_RA8875(int _width, int _height) :  tft(RA8875_CS,RA8875_RESET) {
	width = _width;
	height = _height;
	supportsLayers = true;
}

void Display_ER_RA8875::writeTo(int layer) {
	if (layer==0)
		tft.writeTo(L1);
	else
		tft.writeTo(L2);
}

void Display_ER_RA8875::setTransparentColor(uint16_t color) {
	transparentColor = color;
	tft.setTrasparentColor(getColorLocal(color));
}

void Display_ER_RA8875::clearRect(int16_t x, int16_t y, int16_t w, int16_t h) {
	fillRect(x, y, w, h, transparentColor);
}

void Display_ER_RA8875::layerEffect(int layerEffect) {
	if (layerEffect == 0)
		tft.layerEffect(LAYER1);
	else if (layerEffect == 1)
		tft.layerEffect(LAYER2);
	else if (layerEffect == 2)
		tft.layerEffect(TRANSPARENT);
}

void Display_ER_RA8875::begin()
{
	tft.begin(RA8875_480x272);

	if (supportsLayers)
	{
		tft.useLayers(true);
		layerEffect(1);
		writeTo(0);
		setTransparentColor(getColor(0xE0, 0x38, 0x70));
		tft.fillScreen(getColorLocal(transparentColor));
		writeTo(1);
	}
	tft.fillScreen(RA8875_WHITE);
	tft.setExternalFontRom(ER3303_4,ASCII,ARIAL);
	tft.setFont(EXT);
	tft.setFontFullAlign(false);

}
uint16_t Display_ER_RA8875::getColorLocal(uint16_t color)
{
	return ((color & 0x3C00) << 2) | ((color & 0x3E0) << 1) | ((color & 0x1F));
}

void Display_ER_RA8875::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
	changeMode(GRAPHIC);
	tft.drawRect(x, y, w, h, getColorLocal(color));
}

void Display_ER_RA8875::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
	changeMode(GRAPHIC);
	//tft.fillRect(x, y, w, h, getColorLocal(color));
	tft.fillRect(x, y, w, h, getColorLocal(color));
}

void Display_ER_RA8875::fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t radius, uint16_t color)
{
	changeMode(GRAPHIC);
	//tft.fillRect(x, y, w, h, getColorLocal(color));
	tft.fillRoundRect(x, y, w, h, radius, getColorLocal(color));
}

void Display_ER_RA8875::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
	tft.fillTriangle(x0, y0, x1, y1, x2, y2, getColorLocal(color));
}

void Display_ER_RA8875::rectHelper(int16_t x, int16_t y, int16_t x2, int16_t y2, uint16_t color, bool filled)
{

}
/*
void Display_ER_RA8875::drawText(int16_t x, int16_t y, int16_t size, const char* text, int16_t len, uint16_t color, int16_t rotation)
{
	changeMode(TEXT);
	tft.setCursor(x, y);
	tft.setTextColor(getColorLocal(color));
	if (size < 20)
		//tft.setFontScale(0);
		tft.setFontSize(X16, false);
	else if (size < 34)
		tft.setFontSize(X24, false);
	else if (size < 50)
		tft.setFontSize(X32, false);
	else 
		tft.setFontScale(1);
	tft.setFontRotate(rotation?true:false);
	tft.println(text);
}
*/
void Display_ER_RA8875::drawText(int16_t x, int16_t y, int16_t w, int16_t h, TextAlignHorizontal horAlign, TextAlignVertical verAlign, int16_t size, const char* text, int16_t len, uint16_t color) 
{
	static char buffer[64];
	changeMode(TEXT);
	tft.setTextColor(getColorLocal(color));
	tft.setFontScale(0);

	byte fontSize = X16;

	if (size < 20) {
		fontSize = X16;
		size = 16;
	} else if (size < 34) {
		fontSize = X24;
		size = 24;
	} else if (size < 50) {
		fontSize = X32;
		size = 32;
	} else {
		fontSize = X24;
		tft.setFontScale(1);
		size = 48;
	}

	tft.setFontSize((RA8875tsize)fontSize, false);
	size = size*0.59;

	if (w == 0 || h == 0 || false) //
	{
		tft.setCursor(x + 2, y + 2);
		tft.println(text);
	}
	else 
	{
		if (verAlign == TextAlignTop && horAlign == TextAlignLeft && false) {
			tft.setCursor(x, y);
			tft.setActiveWindow(x, x+w, y, y+h);
			tft.println(text);
			tft.setActiveWindow(0, tft.width()-1, 0, tft.height()-1);
		}
		else
		{
			int curX = 0;
			int curY = 0;
			int wordStartPos = 0;
			int currentLine = 0;
			int wordWidth = 0;
			for (int i=0;i<len+1;i++) {
				if (text[i] == ' ' || text[i] == 0) {
					//int wordLength = i - wordStartPos;
					//if (wordLength * size + curX > w && curX>0) {
					if (wordWidth + curX > w && curX>0) {
						currentLine = currentLine+1;
						curX = 0;
					}
					wordStartPos = i+1;
					//curX += wordLength * size;
					curX += wordWidth;
					curX += size * 0.3;
					wordWidth = 0;
				}
				else {
					wordWidth += TEXT_WIDTH(text[i]);
				}
			}
		
			if (verAlign == TextAlignMiddle)
				curY = (h - (currentLine+1)*size*1.7*1.2 + size*0.2)/2;
			else if  (verAlign == TextAlignBottom)
				curY = (h - (currentLine+1)*size*1.7*1.2 + size*0.2);
			else
				curY = 0;
		
			int lineStartPos = 0;
			curX = 0;
			wordStartPos = 0;
			wordWidth = 0;
			for (int i=0;i<len+1;i++) {
				if (text[i] == ' ' || text[i] == 0) {
					//int wordLength = i - wordStartPos;
					//if ((wordLength * size + curX > w && curX>0)) {
					if ((wordWidth + curX > w && curX>0)) {
						if (horAlign == TextAlignCenter)
							curX = (w - curX)/2;
						else if  (horAlign == TextAlignRight)
							curX = (w - curX);
						else
							curX = 0;
						int tmpWordStartPos = lineStartPos;
						for (int j=lineStartPos;j<wordStartPos;j++) {
							if (text[j] == ' ' || text[j] == 0) {
								tft.setCursor(x + curX, y + curY);
								int len = min(63, j-tmpWordStartPos);
								memcpy(buffer, text + tmpWordStartPos, len);
								buffer[len] = 0;
								tft.println(buffer);
							}
						}
						curX = 0;
						curY += size*1.7 * 1.2;
						lineStartPos = wordStartPos;
					}
					//curX += wordLength * size;
					curX += wordWidth;
					curX += size * 0.3; // word spacing
					wordStartPos = i+1;
					wordWidth = 0;
				}
				else {
					wordWidth += TEXT_WIDTH(text[i]);
				}
			}
			if (horAlign == TextAlignCenter)
				curX = (w - curX)/2;
			else if  (horAlign == TextAlignRight)
				curX = (w - curX);
			else
				curX = 0;
			int tmpWordStartPos = lineStartPos;

			for (int j=lineStartPos;j<len+1;j++) {
				if (text[j] == ' ' || text[j] == 0) {
					tft.setCursor(x + curX, y + curY);
					int len = min(63, j-tmpWordStartPos);
					memcpy(buffer, text + tmpWordStartPos, len);
					buffer[len] = 0;
					tft.println(buffer);
				}
			}
		}
	}
}

void Display_ER_RA8875::changeMode(RA8875modes _mode) {
	if (mode!=_mode) {
		tft.changeMode(_mode);
		mode = _mode;
	}
}
