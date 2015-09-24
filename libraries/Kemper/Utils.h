
#pragma once

#include "Kemper_namespace.h"
#include "Arduino.h"

//#define abs(x) ((x)<0?(-(x)):(x))
//#define KEMPER_DEBUG

BEGIN_KEMPER_NAMESPACE

#ifdef ARDUINO
void debug(const __FlashStringHelper* str);
void debug2(const __FlashStringHelper* str);
#endif

void debug(const char* str);
void debug(long x);
void debug(int x);
void debug(byte a);
void debug2(const char* str);

int getColor(byte r, byte g, byte b);

char *ftoa(char *a, double f, int precision);

END_KEMPER_NAMESPACE