/*!
 *  @file       Kemper.h
 *  Project     Arduino Kemper MIDI Library
 *  @brief      Kemper MIDI Library for the Arduino
 *  @version    1.0
 *  @author     Ersin Basaran
 *  @date       16/07/15
 *  @license    GPL v3.0
*/
#pragma once 

#include "Arduino.h"
#include "MIDI/MIDI.h"

#include "AbstractKemper.h"

#include "Kemper_namespace.h"
#include "Stomp.h"
#include "utils.h"

//#define KEMPER_DEBUG 1

BEGIN_KEMPER_NAMESPACE

class Kemper : public AbstractKemper
{
	private:
		char lineBuffer[512];
		const byte* data;

		byte senseReceived;

	public:
		Kemper();

		static void onSysExStatic(byte* buffer, unsigned int size);
		void onSysEx(byte* buffer, unsigned int size);

		static void onControlChangeStatic(byte channel, byte number, byte value);
		void onControlChange(byte channel, byte number, byte value);

		static void onProgramChangeStatic(byte channel, byte number);
		void onProgramChange(byte channel, byte number);

		static void onActiveSensingStatic();
		void onActiveSensing();

		void begin();
		void read();
		void syncState();

		void getDelay();
		void getReverb();
		void getState();
		void getTuner();
		void getTempo();
		void getRigName();
		void getMode();
		void getStringParam(byte page, byte number);
		void getParam(byte page, byte number);

		void tunerOn();
		void tunerOff();
		void tapOn();
		void tapOff();
		void setRig(byte rig);
		
		void looperRecordPlayDown();
		void looperRecordPlayUp();
		void looperReverseDown();
		void looperReverseUp();
		void looperHalfTimeDown();
		void looperHalfTimeUp();
		void looperUndoDown();
		void looperUndoUp();
		void looperStopEraseDown();
		void looperStopEraseUp();
		void looperTriggerDown();
		void looperTriggerUp();

		void setParam(byte page, byte number, int val);

		void sendControlChange(byte data1, byte data2);
		void sendProgramChange(byte data1, byte data2);
		void sendSysEx(byte command, byte page, byte number);
		void sendSysExExtended(byte command, unsigned int page, unsigned int number);
		void sendSysExExtended(byte command, byte b1, byte b2, byte b3, byte b4, byte b5);
		void sendStream();

		void toggleStomp(byte idx);
		
		void setPerformance(int idx, int slot = 0);
		void onPerformanceChanged();
};



END_KEMPER_NAMESPACE

