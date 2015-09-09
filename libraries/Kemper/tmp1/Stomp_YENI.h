

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

struct KemperParam
{
	char name[32];
	const KemperParamOption* options;
	int minValue;
	int maxValue;
};

struct StompInfo
{
	int type;
	Color color;
	char name[20];
	const KemperParam* params;
};

struct StompState
{
	bool active;
	bool dirty;
	StompInfo info;
};

#define AllStompsCount 64

extern const StompInfo AllStomps[AllStompsCount];


bool getStompInfo(int stompId, int id, StompInfo *info);


END_KEMPER_NAMESPACE