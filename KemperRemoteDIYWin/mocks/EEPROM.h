
#pragma once

#include "Arduino.h"

#define WINDOWS_EEPROM_SIZE 4096
//
//class C_EEPROM
//{
//private:
//	byte _data[4096];
//public:
//	C_EEPROM();
//	template< typename T > void put(int address, T data);
//	template< typename T > void get(int address, T data);
//};
//
//extern C_EEPROM EEPROM;

class EEPROMClass {
private:
	uint8_t eepromBuffer[WINDOWS_EEPROM_SIZE];
public:
	EEPROMClass() {
		memset(eepromBuffer, -1, sizeof(eepromBuffer));
		FILE *file = fopen("eeprom.bin", "rb");
		if (file) {
			fread(eepromBuffer, sizeof(eepromBuffer), 1, file);
			fclose(file);
		}
	}

	template< typename T > T &get(int idx, T &t) {
		uint8_t *ptr = (uint8_t*)&t;
		int len = sizeof(T);
		memcpy(ptr, eepromBuffer + idx, len);
		return t;
	}

	template< typename T > const T &put(int idx, const T &t) {
		const uint8_t *ptr = (const uint8_t*)&t;
		int len = sizeof(T);
		memcpy(eepromBuffer + idx, ptr, len);

		FILE *file = fopen("eeprom.bin", "wb");
		fwrite(eepromBuffer, sizeof(eepromBuffer), 1, file);
		fclose(file);

		return t;
	}
};

//	
//}
//
//template< typename T > void C_EEPROM::get(int address, T data) {
//	memcpy(&data, _data + address, sizeof(T));


static EEPROMClass EEPROM;

