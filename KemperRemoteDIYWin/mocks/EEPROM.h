
#pragma once


class C_EEPROM
{
public:
	void put(int address, void* data);
	void get(int address, void* data);
};

extern C_EEPROM EEPROM;