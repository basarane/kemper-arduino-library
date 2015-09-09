
#pragma once 

#include "AbstractDisplaySerializer.h"

class VirtualDisplaySerializer : public AbstractDisplaySerializer
{
private:
	HardwareSerial* Serial;
public:
	VirtualDisplaySerializer(HardwareSerial* _Serial);
	int transferByte(uint8_t a);
	int transfer(uint16_t a);
	int transfer(uint32_t a);
	int transferString(const char* text);

	void start();
	void end();
};

