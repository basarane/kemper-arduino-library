
#include "VirtualDisplaySerializer.h"

VirtualDisplaySerializer::VirtualDisplaySerializer(HardwareSerial* _Serial)
{
	Serial = _Serial;
}

int VirtualDisplaySerializer::transferByte(uint8_t a) {
	if (a>0x7f) {
		//KEMPER_NAMESPACE::debug("ERROR!!! SPI byte");
		//KEMPER_NAMESPACE::debug(a);	
	}
	Serial->write(a);
	//Serial->write(a >> 4);
	//Serial->write(a & 0xf);
	return 0;
}

int VirtualDisplaySerializer::transfer(uint16_t a) {
	transferByte((uint8_t)(a>>7));
	transferByte((uint8_t)(a & 0x7f));
	//transferByte((uint8_t)(a>>8));
	//transferByte((uint8_t)(a & 0xff));
	return 0;
}

int VirtualDisplaySerializer::transfer(uint32_t a) {
	transfer((uint16_t)(a>>14));
	transfer((uint16_t)(a & 0x3fff));
	return 0;
}

int VirtualDisplaySerializer::transferString(const char* text) {
	return 0;
}

void VirtualDisplaySerializer::start() {
	Serial->write(0xf2);
}

void VirtualDisplaySerializer::end() {
	Serial->write(0xff);
	Serial->flush();
}

