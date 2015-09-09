#pragma once

#include "Arduino.h"
#include "Utils.h"
#include "AbstractDisplaySerializer.h"

class AbstractDisplaySerializer 
{
public:
	virtual int transferByte(uint8_t a) = 0;
	virtual int transfer(uint16_t a) = 0;

	virtual void start() = 0;
	virtual void end() = 0;
};

