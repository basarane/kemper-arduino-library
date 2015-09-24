
#pragma once

#include "AbstractKemper.h"
#include <ExpressionPedal.h>

#include "Utils.h"
#include <EEPROM.h>

BEGIN_KEMPER_NAMESPACE 

#define EXPRESSION_PEDAL_1_PIN 0
#define EXPRESSION_PEDAL_2_PIN 1
#define EXPRESSION_PEDAL_3_PIN 2
#define EXPRESSION_PEDAL_4_PIN 3

#define PARAMETER_BUFFER_ALL_SIZE 1024
#define PARAMETER_BUFFER_SIZE 64

#define SWITCH_RIG_START 0
#define SWITCH_STOMP_START 5
#define SWITCH_STOMP_COUNT 4
#define SWITCH_RIG_COUNT 5
#define SWITCH_TAP 9
#define SWITCH_TUNER 10
#define SWITCH_LOOPER 11
#define SWITCH_UP 12
#define SWITCH_DOWN 13
#define SWITCH_COUNT 14

#define BROWSE_PAGE_COUNT ((byte)ceil((float)RIG_COUNT/SWITCH_RIG_COUNT))

#define REMOTE_PARAMETER_STATE_PARAMETER 0
#define REMOTE_PARAMETER_STATE_EXPRESSION 1
#define REMOTE_PARAMETER_STATE_VALUE 2

#define REMOTE_STATE_NORMAL 1
#define REMOTE_STATE_STOMP_ASSIGN 2
#define REMOTE_STATE_RIG_ASSIGN 3
#define REMOTE_STATE_LOOPER 4
#define REMOTE_STATE_TEMPO_DETECTION 5
#define REMOTE_STATE_EXPRESSION_CALIBRATE 6
#define REMOTE_STATE_STOMP_PARAMETER 7
#define REMOTE_STATE_STOMP_PARAMETER_LOAD 8
#define REMOTE_STATE_STOMP_PARAMETER_POST_LOAD 9

#define LED_COUNT 26

#define PERFORM_SLOT_COUNT 5 

#define EXPRESSION_PEDAL_COUNT 1

#define EXPRESSION_PEDAL_MODE_PARAMETER -1

extern int ExpressionPedalModes[4];

struct KemperRemoteState
{
	int state;
	int previousState;
	byte currentPage;
	byte currentPerformance;
	byte currentSlot;
	bool isSaved;
	int parameterState;
	byte* currentParameters;
	bool currentParametersChanged;
	unsigned long expPedalState;
};


class KemperRemote
{
private:
	byte currentRig;
	byte stompAssignment[SWITCH_RIG_COUNT][SWITCH_STOMP_COUNT];
	byte stompAssignmentPerform[PERFORM_SLOT_COUNT][SWITCH_STOMP_COUNT];

	int currentSwitch;
	bool switchStates[SWITCH_COUNT];
	unsigned long switchDownStart;
	byte saveUpDown;
	bool initialStompStates[KEMPER_STOMP_COUNT];
	bool changedStomps[KEMPER_STOMP_COUNT];
	byte rigAssignRig;

	byte rigMap[RIG_COUNT];

	int lastStompTypes[KEMPER_STOMP_COUNT];

	int oldStompParameters[KEMPER_STOMP_COUNT][MAX_KEMPER_PARAM_LENGTH][2];
	int newStompParameters[KEMPER_STOMP_COUNT][MAX_KEMPER_PARAM_LENGTH][2];
	byte parameterBuffer[PARAMETER_BUFFER_SIZE];
	int eepromParameterBufferStart;
	void updateCurrentParameter(byte perf, byte slot);

	void refreshStompAssignment();
	void checkSwitchDownLong();
	void checkUpDownScroll();
	void checkStompChanges();
	void updateLeds();

	bool changeExpPedalMode();
	void updateExpPedalModes();
public:

	ExpressionPedal expPedals[EXPRESSION_PEDAL_COUNT];
	int calibratePedalId;

	byte leds[LED_COUNT * 3];
	byte rigAssignSwitch;
	byte currentStompAssignment[SWITCH_STOMP_COUNT];
	KemperRemoteState state;
	
	AbstractKemper* kemper;
	KemperRemote(AbstractKemper* _kemper); 
	void read();

	void onSwitchDown(int sw);
	void onSwitchUp(int sw);

	void onStompDown(int switchIdx);
	void onStompUp(int switchIdx);
	void onRigDown(int switchIdx);
	void onRigUp(int switchIdx);

	void assignStomps(byte switchId, byte assign);
	void assignRig(byte switchId, byte rig);

	byte getRigIndex(byte rig);

	void save();
};

END_KEMPER_NAMESPACE