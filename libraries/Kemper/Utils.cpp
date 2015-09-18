
#include "Utils.h"


#ifndef ARDUINO
#define ITOA _itoa
#else
#define ITOA itoa
#endif

#ifdef ARDUINO

void KEMPER_NAMESPACE::debug(const __FlashStringHelper* str) {
#ifndef KEMPER_DEBUG
	Serial.write(0xF1);
	Serial.print(str);
	Serial.write(0xFF);
	Serial.flush();
#endif
}


void KEMPER_NAMESPACE::debug2(const __FlashStringHelper* str) {
#ifdef KEMPER_DEBUG
	Serial.println(str);
#endif
}
#endif 

void KEMPER_NAMESPACE::debug(const char* str) {
#ifndef KEMPER_DEBUG
	Serial.write(0xF1);
	Serial.print(str);
	Serial.write(0xFF);
	Serial.flush();
#endif
}

void KEMPER_NAMESPACE::debug2(const char* str) {
#ifdef KEMPER_DEBUG
	Serial.println(str);
#endif
}


void KEMPER_NAMESPACE::debug(int x) {
#ifndef KEMPER_DEBUG
	Serial.write(0xF1);
	Serial.print(x);
	Serial.write(0xFF);
	Serial.flush();
#endif
}

void KEMPER_NAMESPACE::debug(byte a) {
#ifndef KEMPER_DEBUG
	Serial.write(0xF1);
	Serial.print(a);
	Serial.write(0xFF);
	Serial.flush();
#endif
}

int KEMPER_NAMESPACE::getColor(byte r, byte g, byte b) {
	//return (((int)r>>3)<<11) | (((int)g>>2)<<5) | (b>>3); // 16 bit
	return (((int)r>>4)<<10) | (((int)g>>3)<<5) | (b>>3); // 14 bit
}

char *KEMPER_NAMESPACE::ftoa(char *a, double f, int precision)
{
 long p[] = {0,10,100,1000,10000,100000,1000000,10000000,100000000};
 
 char *ret = a;
 long heiltal = (long)f;
 ITOA(heiltal, a, 10);
 while (*a != '\0') a++;
 *a++ = '.';
 long desimal = abs((long)((f - heiltal) * p[precision]));
 ITOA(desimal, a, 10);
 return ret;
}
