#pragma once

#include "Arduino.h"
#include "RtMidi.h"

class MidiInWrapper
{
private:
public:
	std::vector< unsigned char > *lastMessage;
    void (*mControlChangeCallback)(byte channel, byte, byte);
    void (*mProgramChangeCallback)(byte channel, byte);
    void (*mSystemExclusiveCallback)(byte * array, unsigned size);
    void (*mActiveSensingCallback)(void);

	MidiInWrapper();
	RtMidiIn midiIn;
	void begin(byte channel);
	void setHandleSystemExclusive(void (*fptr)(byte * array, unsigned size))			{mSystemExclusiveCallback = fptr;}
	void setHandleControlChange(void (*fptr)(byte channel, byte number, byte value))	{mControlChangeCallback = fptr;}
	void setHandleProgramChange(void (*fptr)(byte channel, byte number))				{mProgramChangeCallback = fptr;}
	void setHandleActiveSensing(void (*fptr)(void))										{mActiveSensingCallback = fptr;}
	void read();
	const byte* getSysExArray();
	unsigned getSysExArrayLength();
};

class MidiOutWrapper
{
private:
	std::vector<unsigned char> message;
public:
	RtMidiOut midiOut;
	void begin(byte channel);
	void sendSysEx(unsigned inLength, const byte* inArray, bool inArrayContainsBoundaries = false);
	void send(byte inType, byte inData1, byte inData2, byte inChannel);
};

