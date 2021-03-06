
#pragma once

typedef unsigned char byte;

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
//#define abs(x) ((x)>0?(x):-(x))
#define PROGMEM

#define PI 3.1415926535897932384626433832795

#define memcpy_P memcpy
#define strcpy_P strcpy
#define pgm_read_word_near(a) (int)(*(a))
#define pgm_read_byte_near(a) (byte)(*(a))
#define pgm_read_float_near(a) (float)(*(a))

#define __FlashStringHelper char

#define F(x) x

unsigned long millis();

#include <stdio.h>
#include "TypesMock.h"
//#include "wstring.h"
#include <math.h>
#include "WindowsSerial.h"

class HardwareSerial
{
public:
	void write(byte a);
	void print(const char *s);
	void print(int s);
	void println(const char *s);
	void println(int s);
	void flush();
};

extern HardwareSerial Serial;
int analogRead(int pin);