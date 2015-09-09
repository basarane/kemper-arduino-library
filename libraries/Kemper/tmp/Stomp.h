

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

struct StompInfo
{
	int type;
	Color* color;
	String name;
};

struct StompState
{
	bool active;
	bool dirty;
	StompInfo* info;
};

#define AllStompsCount 63

extern StompInfo AllStomps[AllStompsCount];


StompInfo* getStompInfo(int stompId, int id);


END_KEMPER_NAMESPACE