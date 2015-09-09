
#pragma once 

#include "AbstractDisplaySerializer.h"
#include "WindowsSerial.h"

class SerialDisplaySerializer : public AbstractDisplaySerializer
{
public:
	SerialDisplaySerializer(char *portName);
	WindowsSerial Serial;
	int transferByte(uint8_t a);
	int transfer(uint16_t a);
	int transferString(const char *text);

	void start();
	void end();
};

