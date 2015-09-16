#pragma once

#include "VirtualDisplay.h"

#include "Kemper_namespace.h"

#include "AbstractKemper.h"
#include "KemperRemote.h"
#include "AbstractDisplay.h"

#include "Utils.h"

BEGIN_KEMPER_NAMESPACE

class KemperRemoteDisplay {
public: 
	KemperState lastKemperState;
	KemperRemoteState lastKemperRemoteState;
	KemperRemoteDisplay(AbstractKemper* _kemper, KemperRemote* _kemperRemote, AbstractDisplay* _display);
	AbstractKemper* kemper;
	KemperRemote* kemperRemote;
	AbstractDisplay* display;

	unsigned long lastTempoTime;
	unsigned long lastTunerTime;
	unsigned long lastDisplay;
	unsigned int writeToLayer;
	int firstRun;

	void welcome();
	void draw();
};

END_KEMPER_NAMESPACE