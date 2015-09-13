#include <VirtualDisplaySerializer.h>
#include <VirtualDisplay.h>
#include <SPI.h>
#include <RA8875.h>
#include <MemoryFree.h>

#include <EEPROM.h>
#include "MIDI.h"
#include "RA8875.h"

#include "Kemper_Namespace.h"
#include <Kemper.h>
#include <KemperRemote.h>
#include <KemperRemoteDisplay.h>
#include "Display_ER_RA8875.h"
#include <Utils.h>
#include "MultiButton.h"

#include "Tlc5940.h"

MultiButton buttons;

bool lastValues[MAX_BUTTON_COUNT];

int rigNo = 1;

#define LED 13 

#define TUNER_LOW 2
#define TUNER_OK 3
#define TUNER_HIGH 4

#define ENABLE_VIRTUAL_DISPLAY

USING_NAMESPACE_KEMPER

Kemper kemper;
KemperRemote kemperRemote(&kemper);

byte lastLeds[LED_COUNT * 3];

#ifdef ENABLE_VIRTUAL_DISPLAY
VirtualDisplaySerializer displaySerializer(&Serial);
VirtualDisplay displayProvider(&displaySerializer, 480, 272);
KemperRemoteDisplay display(&kemper, &kemperRemote, &displayProvider);
#endif
/*
VirtualDisplaySerializer displaySerializer2(&Serial3);
VirtualDisplay displayProvider2(&displaySerializer2, 480, 272);
KemperRemoteDisplay display2(&kemper, &kemperRemote, &displayProvider2);
*/
Display_ER_RA8875 displayProvider2(480, 272);
KemperRemoteDisplay display2(&kemper, &kemperRemote, &displayProvider2);

void setup()
{
	pinMode(LED, OUTPUT);
	/*
	for (int i=0;i<SWITCH_COUNT;i++)
		pinMode(switchPins[i], INPUT); //INPUT_PULLUP
	*/
	Serial.begin(921600);
	//Serial3.begin(921600);
	kemper.begin();

	displayProvider2.begin();
	buttons.attach(14, 4, 5, 6, 7);

	Tlc.init();

	//kemper.setRig(0);
}

void loop()
{
	static unsigned long reportTime = 0;
	static unsigned long ledTime = 0;
	static unsigned long tlcLedTime = 0;

	kemper.read();
	kemperRemote.read();

	if (Serial.available()) {
		int sw = Serial.read() - 1;
		while (!Serial.available()) {
		}
		int pressed = Serial.read();

		if (pressed)
			kemperRemote.onSwitchDown(sw);
		else
			kemperRemote.onSwitchUp(sw);
	}

	buttons.update();
	for (int i = 0; i < buttons.buttonCount; i++) {
		if (buttons.values[i] != lastValues[i]) {
			lastValues[i] = buttons.values[i];
			if (buttons.values[i]) {
				kemperRemote.onSwitchDown(i);
			}
			else {
				kemperRemote.onSwitchUp(i);
			}
		}
	}


#if !defined(KEMPER_DEBUG) && defined(ENABLE_VIRTUAL_DISPLAY)
	if (millis() - ledTime > 50) {
		bool isChanged = false;
		for (int i = 0; i < LED_COUNT * 3; i++)
		{
			if (kemperRemote.leds[i] != lastLeds[i]) {
				isChanged = true;
				lastLeds[i] = kemperRemote.leds[i];
			}
		}
		if (isChanged)
		{
			Serial.write(0xF0);
			for (int i = 0; i < LED_COUNT * 3; i++) {
				Serial.write(kemperRemote.leds[i]);
			}
			Serial.write(0xFF);
			Serial.flush();
		}
		ledTime = millis();
	}
#endif

if (millis() - tlcLedTime > 50) {
	int ledShift = 3;
	for (int i=0;i<SWITCH_RIG_COUNT;i++)
	{
		Tlc.set(i, ((int)kemperRemote.leds[i*3]) << ledShift);
	}
	int tlcOffset = SWITCH_RIG_COUNT;
	int ledOffset = tlcOffset * 3;
	//Serial.print("LedSet\n");
	for (int i=0;i<SWITCH_STOMP_COUNT;i++)
	{
		for (int j=0;j<2;j++) {
			for (int t=0;t<4;t++) {
				int value = ((int)kemperRemote.leds[i*12 + j*6 + ledOffset + t]) << ledShift;
				Tlc.set(i*8 + j*4 + tlcOffset + t, value);
			}
		}
	}
	tlcOffset = SWITCH_RIG_COUNT + SWITCH_STOMP_COUNT * 8;
	ledOffset = (SWITCH_RIG_COUNT + SWITCH_STOMP_COUNT*4)*3;
	for (int i=0;i<5;i++)
		Tlc.set(tlcOffset + i, ((int)kemperRemote.leds[ledOffset+i*3+1]) << ledShift);

	Tlc.update();
	tlcLedTime = millis();
}


#if !defined(KEMPER_DEBUG) && defined(ENABLE_VIRTUAL_DISPLAY)
	display.draw();
#endif
	display2.draw();

#if defined(ENABLE_VIRTUAL_DISPLAY)
	static unsigned long memoryDebugTime = 0;
	static unsigned int perIdx = 0;
	if (millis() - memoryDebugTime > 5000 && true) { 
		debug (F("Free memory / alive time = "));
		debug(freeMemory ());
		debug((int)(millis() / 1000));
		memoryDebugTime = millis();
	}
#endif
}

