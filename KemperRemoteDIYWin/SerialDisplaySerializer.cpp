
#include "SerialDisplaySerializer.h"

SerialDisplaySerializer::SerialDisplaySerializer(char *portName) {
	Serial.Connect(portName);
}

int SerialDisplaySerializer::transferByte(uint8_t a) {
	if (a>0xff) {
		KEMPER_NAMESPACE::debug("ERROR!!! SPI byte");
		KEMPER_NAMESPACE::debug(a);	
	}
	unsigned char data[2] = {a >> 4, a & 0xf};
	Serial.WriteData(data, 2);
	return 0;
}

int SerialDisplaySerializer::transfer(uint16_t a) {
	transferByte((uint8_t)(a>>8));
	transferByte((uint8_t)(a & 0xff));
	return 0;
}

void SerialDisplaySerializer::start() {
	unsigned char data[1] = {0xf2};
	Serial.WriteData(data, 1);
}

void SerialDisplaySerializer::end() {
	unsigned char data[1] = {0xff};
	Serial.WriteData(data, 1);
}

int SerialDisplaySerializer::transferString(const char *text) {
	return 0;
}
