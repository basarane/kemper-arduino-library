
#include "VirtualDisplaySerializer.h"

int VirtualDisplaySerializer::transferByte(uint8_t a) {
	if (a>0xff) {
		KEMPER_NAMESPACE::debug("ERROR!!! SPI byte");
		KEMPER_NAMESPACE::debug(a);	
	}
	Serial.write(a >> 4);
	Serial.write(a & 0xf);
	return 0;
}

int VirtualDisplaySerializer::transfer(uint16_t a) {
	transferByte((uint8_t)(a>>8));
	transferByte((uint8_t)(a & 0xff));
	return 0;
}

void VirtualDisplaySerializer::start() {
	Serial.write(0xf2);
}

void VirtualDisplaySerializer::end() {
	Serial.write(0xff);
	Serial.flush();
}

