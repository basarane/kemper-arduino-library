#pragma once

#include "Arduino.h"
#include "AbstractKemper.h"

#include "Kemper_namespace.h"
#include "Utils.h"

BEGIN_KEMPER_NAMESPACE

#define EXPRESSION_PEDAL_THRESHOLD 10
#define EXPRESSION_PEDAL_HISTORY_LENGTH 10
#define EXPRESSION_PEDAL_CALIBRATE_MARGIN 10

#define EXPRESSION_MODE_STR(m) ((m)==CC_VOLUME?"Volume":((m)==CC_WAH?"Wah":((m)==CC_PITCH?"Pitch":((m)==EXPRESSION_PEDAL_MODE_PARAMETER?"Param":("")))))

class ExpressionPedal
{
private:
	int lastValues[EXPRESSION_PEDAL_HISTORY_LENGTH];
	int lastValuesIndex;
	int lastCheckedValue;
	unsigned long lastRead;
public:
	int simValue;
	int read();
	void begin(int pin);
	int value;
	int pin;
	int minValue;
	int maxValue;
	int calibratedValue();
	int mode;
	void calibrate();
	bool isCalibrated();
	bool isChanged(int divider);
};

END_KEMPER_NAMESPACE