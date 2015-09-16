#include "ExpressionPedal.h"

USING_NAMESPACE_KEMPER


void ExpressionPedal::begin(int _pin)
{
	pin = _pin;
	minValue = 1024;
	maxValue = -1;
	lastValuesIndex = 0;
	mode = CC_VOLUME;
	simValue = -1;
	//pinMode(pin, INPUT_PULLUP);
}

int ExpressionPedal::read()
{
	if (millis() - lastRead > 3 + (pin%4)) // analog read'lerde faz farki olsun diye
	{
		int raw = analogRead(pin);
		if (simValue >= 0)
			raw = simValue;
		value = raw;
		if (value > EXPRESSION_PEDAL_THRESHOLD) {
			lastValues[lastValuesIndex] = value;
			lastValuesIndex = (lastValuesIndex+1)%EXPRESSION_PEDAL_HISTORY_LENGTH;
		}
		lastRead = millis();
	}
	return value;
}

void ExpressionPedal::calibrate() {
	if (value>=EXPRESSION_PEDAL_THRESHOLD) {
		int minV = -1;
		int maxV = 1024;
		for (int i=0;i<EXPRESSION_PEDAL_HISTORY_LENGTH;i++)
		{
			if (lastValues[i] > EXPRESSION_PEDAL_THRESHOLD)
			{
				minV = max(minV, lastValues[i]);
				maxV = min(maxV, lastValues[i]);
			}
		}
		minValue = min(minValue, minV + EXPRESSION_PEDAL_CALIBRATE_MARGIN);
		maxValue = max(maxValue, maxV - EXPRESSION_PEDAL_CALIBRATE_MARGIN);
	}
}

int ExpressionPedal::calibratedValue() {
	float val = 0;
	if (maxValue > minValue)
	{
		val = 1024.0*(value - minValue) / (maxValue - minValue);
		val = max(val, 0);
		val = min(val, 1023);
	}
	return (int)val;
}

bool ExpressionPedal::isCalibrated() {
	return minValue<maxValue;
}

bool ExpressionPedal::isChanged(int divider) {
	if (lastCheckedValue/divider!=calibratedValue()/divider) {
		//debug("EXP changed");
		//debug(lastCheckedValue/divider);
		//debug(calibratedValue()/divider);
		lastCheckedValue = calibratedValue();
		return true;
	}
	return false;
}