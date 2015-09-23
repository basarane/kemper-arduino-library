

#pragma once


#include "Arduino.h"

#include "Kemper_namespace.h"

BEGIN_KEMPER_NAMESPACE

struct Color
{
	byte r;
	byte g;
	byte b;
};

struct KemperParamOption
{
	int value;
	char name[20];
};

struct KemperParamValue
{
	byte id;
	float minValue;
	float maxValue;
	int maxParam;
	char suffix[3];
	bool exponential;
	bool special;
};

struct PGM_KemperParam
{
	int number;
	char name[32];
	int optionCount;
	const KemperParamOption* const* options;
	const KemperParamValue* value;
};

struct PGM_StompInfo
{
	int type;
	Color color;
	char name[20];
	int paramCount;
	const PGM_KemperParam* const* params;
	byte isExpWah;
	byte isExpPitch;
};

struct KemperParam
{
	int number;
	char name[32];
	int optionCount;
};

struct StompInfo
{
	unsigned int PGM_index;
	int type;
	Color color;
	char name[20];
	int paramCount;
	byte isExpWah;
	byte isExpPitch;
};

struct StompState
{
	bool active;
	bool dirty;
	StompInfo info;
};


#define NUMBER_OF_PARAMS_IN_LIST 5
#define NUMBER_OF_OPTIONS_IN_LIST 5

struct PartialParameter
{
	int paramCount;
	int currentParam;
	int totalParamCount;
	int startParamIndex;

	int optionCount;
	int currentOption;
	int currentValue;
	int totalOptionCount;
	int startOptionIndex;
	bool isActive;

	StompInfo* stompInfo;
	int stompIdx;
	int stompType;
	KemperParam params[NUMBER_OF_PARAMS_IN_LIST];
	KemperParamOption options[NUMBER_OF_PARAMS_IN_LIST];
	KemperParamValue valueType;
};

#define AllStompsCount 64

#define ColorNone {0xCC, 0xCC, 0xCC}
#define ColorWhite {0xFF, 0xFF, 0xFF}
#define ColorOrange {0xFF, 0xA5, 0x00}
#define ColorYellow {0xFF, 0xFF, 0x00}
#define ColorCyan {0x00, 0xFF, 0xFF}
#define ColorBlue {0x00, 0x00, 0xFF}
#define ColorPurple {0x80, 0x00, 0x80}
#define ColorGreen {0x00, 0xFF, 0x00}
#define ColorLightGreen {0xA0, 0xFF, 0xA0}
#define ColorRed {0xFF, 0x00, 0x00}


#include "Stomp_consts.h"

bool loadStompInfo(StompInfo *dst, int stompId, int id);
void loadStompParameters(PartialParameter *parameter, StompInfo *info = 0);
bool updateStompParameterValue(PartialParameter *parameter, int value);
int getOptionValue(PartialParameter *parameter, int optionIndex);

void testParam(int a, int b);


END_KEMPER_NAMESPACE