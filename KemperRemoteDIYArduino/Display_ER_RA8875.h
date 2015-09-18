
#ifndef _DISPLAY_ER_RA8875_H
#define _DISPLAY_ER_RA8875_H

#include "AbstractDisplay.h"

#include "RA8875.h"

#define RA8875_INT 23 
#define RA8875_CS 22 

#define RA8875_RESET 24

class Display_ER_RA8875 : public AbstractDisplay {
private:
	RA8875 tft;
	RA8875modes mode;
	uint16_t transparentColor;
	void changeMode(RA8875modes _mode);
	uint16_t getColorLocal(uint16_t color);
 public:
  Display_ER_RA8875(int _width, int _height);
  	void writeTo(int layer);
	void setTransparentColor(uint16_t color);
	void layerEffect(int layerEffect);
	void clearRect(int16_t x, int16_t y, int16_t w, int16_t h);
  
  void begin();

  void    drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
  void    fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
  void    fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t radius, uint16_t color);
  void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);

  //void    drawText(int16_t x, int16_t y, int16_t size, const char* text, int16_t len, uint16_t color, int16_t rotation);
  //void    drawText(int16_t x, int16_t y, int16_t size, const char* str, int16_t len, uint16_t color, int16_t rotation, int letterCount);
  void drawText(int16_t x, int16_t y, int16_t w, int16_t h, TextAlignHorizontal horAlign, TextAlignVertical verAlign, int16_t size, const char* text, int16_t len, uint16_t color);

  void rectHelper  (int16_t x, int16_t y, int16_t x2, int16_t y2, uint16_t color, bool filled);

};

#endif