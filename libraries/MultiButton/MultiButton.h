
#pragma once

#include "Arduino.h"

#define DEBOUNCED_STATE 0
#define UNSTABLE_STATE  1
#define STATE_CHANGED   3

#define MAX_BUTTON_COUNT 16

class MultiButton
{
protected:
	int buttonPin;
	int pinDS;
	int pinSTCP;
	int pinSHCP;
	uint8_t buttonState[MAX_BUTTON_COUNT];
	unsigned long buttonPreviousMillis[MAX_BUTTON_COUNT];
	unsigned long lastButtonUpdate;
	int buttonInterval;

	void pinClock(int pin);
public:
	int buttonCount;
	bool values[MAX_BUTTON_COUNT];

	MultiButton();
	void attach(int _buttonCount, int _buttonPin, int _pinDS, int _pinSTCP, int _pinSHCP);
	void update();
};
