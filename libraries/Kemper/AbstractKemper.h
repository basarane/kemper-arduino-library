/*!
 *  @file       Kemper.h
 *  Project     Arduino Kemper MIDI Library
 *  @brief      Kemper MIDI Library for the Arduino
 *  @version    1.0
 *  @author     Ersin Basaran
 *  @date       16/07/15
 *  @license    GPL v3.0
*/
#ifndef _ABSTRACT_KEMPER_H
#define _ABSTRACT_KEMPER_H

#include "Arduino.h"

#include "Kemper_namespace.h"
#include "Stomp.h"

BEGIN_KEMPER_NAMESPACE

#define KEMPER_MIDI_CHANNEL 1

#define RIG_COUNT 100
#define KEMPER_STOMP_COUNT 8

#define MODE_NONE 0
#define MODE_TUNER 1
#define MODE_BROWSE 2
#define MODE_PERFORM 3
#define MODE_PROFILE 4

const byte CC_WAH            = 1;
const byte CC_PITCH          = 4;
const byte CC_VOLUME         = 7;
const byte CC_DELAY_MIX      = 68;
const byte CC_DELAY_FEEDBACK = 69;
const byte CC_REVERB_MIX     = 70; 
const byte CC_REVERB_TIME    = 71;
const byte CC_GAIN           = 72;
const byte CC_MONITOR_VOLUME = 73;


struct LooperState
{
	int state;
	bool isHalfTime;
	bool isReversed;
	unsigned long recordPressTime;
	unsigned long erasePressTime;
};

struct KemperState
{
	StompState stomps[KEMPER_STOMP_COUNT];
	int key;
	int tune;
	bool tempoLed;
	bool tempoEnabled;
	int tempo;
	char rigName[20];
	byte currentRig;
	byte mode;

	byte bank;
	byte program;
	byte performance;
	byte slot;
	bool senseReceived;
	
	LooperState looperState;

	unsigned int parameterState;

	char performanceNames[6][20];
};

#define LOOPER_STATE_EMPTY 0
#define LOOPER_STATE_RECORDING 1
#define LOOPER_STATE_PLAYBACK 2
#define LOOPER_STATE_OVERDUB 3

class AbstractKemper
{
	public:
		AbstractKemper();

		KemperState state;
		PartialParameter parameter;
		//int AllParameters[KEMPER_STOMP_COUNT][MAX_KEMPER_PARAM_LENGTH][2];
		int lastStompParam[3];

		void loadPartialParam(int stompIdx);
		void movePartialParam(int paramOffset, int optionOffset);
		void setPartialParamValue(float value);
		virtual void getStompParameter(int stompIdx, int paramNumber) = 0;

		virtual void read() = 0;

		virtual void tunerOn() = 0;
		virtual void tunerOff() = 0;
		virtual void tapOn() = 0;
		virtual void tapOff() = 0;

		virtual void looperRecordPlayDown();
		virtual void looperRecordPlayUp();
		virtual void looperReverseDown();
		virtual void looperReverseUp();
		virtual void looperHalfTimeDown();
		virtual void looperHalfTimeUp();
		virtual void looperUndoDown();
		virtual void looperUndoUp();
		virtual void looperStopEraseDown();
		virtual void looperStopEraseUp();
		virtual void looperTriggerDown();
		virtual void looperTriggerUp();

		virtual void sendControlChange(byte data1, byte data2) = 0;
		virtual void setStompParam(int stompIdx, byte number, int val) = 0;
		virtual void toggleStomp(byte idx) = 0;
		virtual void setPerformance(int idx, int slot = 0) = 0;
		virtual void setRig(byte rig) = 0;
};



END_KEMPER_NAMESPACE

#endif