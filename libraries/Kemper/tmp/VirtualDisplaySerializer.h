
#pragma once 

#include "AbstractDisplaySerializer.h"

class VirtualDisplaySerializer : public AbstractDisplaySerializer
{
public:
	int transferByte(uint8_t a);
	int transfer(uint16_t a);

	void start();
	void end();
};

