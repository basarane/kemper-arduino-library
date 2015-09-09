#include "ExpressionPedal.h"

USING_NAMESPACE_KEMPER

const int wahStepsLength = 26;

float wahSteps[wahStepsLength] = {          
       106.55 ,
       178.25 ,
       262.94 ,
       352.65 ,
       436.03 ,
       521.25 ,
       614.81 ,
       707.77 ,
       787.95 ,
       845.05 ,
       881.44 ,
       908.25 ,
       931.23 ,
       949.04 ,
       964.92 ,
       978.01 ,
       989.75 ,
       999.35 ,
       1005.7 ,
       1010.5 ,
       1014.6 ,
       1017.9 ,
       1019.9 ,
       1021.5 ,
       1022.7 ,
	   1023};


void ExpressionPedal::begin(int _pin)
{
	pin = _pin;
	minValue = 1024;
	maxValue = -1;
	lastValuesIndex = 0;
	mode = CC_VOLUME;
	//pinMode(pin, INPUT_PULLUP);
}

int ExpressionPedal::read()
{
	if (millis() - lastRead > 3 + (pin%4)) // analog read'lerde faz farki olsun diye
	{
		int raw = analogRead(pin);
		//value = linearMap(raw);
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

int ExpressionPedal::linearMap(float raw) 
{
	float stepSize = 1023.0/(wahStepsLength-1);
	value = 0;
	for (int i=0;i<wahStepsLength-1;i++) {
		if (raw >= wahSteps[i] && raw <= wahSteps[i+1]) {
			value = (int)(stepSize * ((raw - wahSteps[i]) * (i+1) + (wahSteps[i+1] - raw) * (i)) / (wahSteps[i+1]-wahSteps[i]));
			break;
		}
	}
	return value;
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