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

#define RIG_COUNT 125
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

	unsigned int parameterState;

	char performanceNames[6][20];
};

class AbstractKemper
{
	public:
		AbstractKemper();

		KemperState state;
		PartialParameter parameter;
		int AllParameters[KEMPER_STOMP_COUNT][MAX_KEMPER_PARAM_LENGTH][2];

		void loadPartialParam(int stompIdx);
		void movePartialParam(int paramOffset, int optionOffset);
		void setPartialParamValue(float value);
		virtual void getStompParameter(int stompIdx, int paramNumber) = 0;

		virtual void read() = 0;

		virtual void tunerOn() = 0;
		virtual void tunerOff() = 0;
		virtual void tapOn() = 0;
		virtual void tapOff() = 0;

		virtual void looperRecordPlayDown() = 0;
		virtual void looperRecordPlayUp() = 0;
		virtual void looperReverseDown() = 0;
		virtual void looperReverseUp() = 0;
		virtual void looperHalfTimeDown() = 0;
		virtual void looperHalfTimeUp() = 0;
		virtual void looperUndoDown() = 0;
		virtual void looperUndoUp() = 0;
		virtual void looperStopEraseDown() = 0;
		virtual void looperStopEraseUp() = 0;
		virtual void looperTriggerDown() = 0;
		virtual void looperTriggerUp() = 0;

		virtual void sendControlChange(byte data1, byte data2) = 0;
		virtual void setStompParam(int stompIdx, byte number, int val) = 0;
		virtual void toggleStomp(byte idx) = 0;
		virtual void setPerformance(int idx, int slot = 0) = 0;
		virtual void setRig(byte rig) = 0;
};



END_KEMPER_NAMESPACE

#endif