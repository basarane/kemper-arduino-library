
#include "MultiButton.h"

void MultiButton::pinClock(int pin) {
	digitalWrite(pin, HIGH);
	digitalWrite(pin, LOW);
}

MultiButton::MultiButton() {
	buttonInterval = 10;
}

void MultiButton::attach(int _buttonCount, int _buttonPin, int _pinDS, int _pinSTCP, int _pinSHCP) {
	buttonCount = _buttonCount;
	buttonPin = _buttonPin;
	pinDS = _pinDS;
	pinSHCP = _pinSHCP;
	pinSTCP = _pinSTCP;
	pinMode(buttonPin, INPUT);
	pinMode(pinDS, OUTPUT);
	pinMode(pinSTCP, OUTPUT);
	pinMode(pinSHCP, OUTPUT);
	digitalWrite(pinDS, LOW);
	for (int i=0;i<buttonCount;i++)
	{
		pinClock(pinSHCP);
	}
	pinClock(pinSTCP);
}

void MultiButton::update() {
	//@ersin - sorun burada. ust uste digitalRead ve analogRead'ler patlatiyor
	//bitbang'e bak
	if (millis() - lastButtonUpdate < 10) {
		return;
	}
	lastButtonUpdate = millis();
	digitalWrite(pinDS, HIGH);
	pinClock(pinSHCP);
	digitalWrite(pinDS, LOW);
	pinClock(pinSTCP);
	for (int i=0;i<buttonCount;i++) {
		bool val = digitalRead(buttonPin);
		buttonState[i] &= ~_BV(STATE_CHANGED);

		if ( val != (bool)(buttonState[i] & _BV(UNSTABLE_STATE)) ) {
			buttonPreviousMillis[i] = millis();
			buttonState[i] ^= _BV(UNSTABLE_STATE);
		} else if ( millis() - buttonPreviousMillis[i] >= buttonInterval && (bool)(buttonState[i] & _BV(DEBOUNCED_STATE)) != val) {
			buttonPreviousMillis[i] = millis();
			buttonState[i] ^= _BV(DEBOUNCED_STATE);
			buttonState[i] |= _BV(STATE_CHANGED);
		}
		bool newVal = buttonState[i] & _BV(DEBOUNCED_STATE);
		if (newVal != values[i])
		{
			values[i] = newVal;
		}
		pinClock(pinSHCP);
		pinClock(pinSTCP);
	}
}