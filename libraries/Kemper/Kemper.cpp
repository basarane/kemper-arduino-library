/*!
 *  @file       Kemper.cpp
 *  Project     Arduino Kemper MIDI Library
 *  @brief      Kemper MIDI Library for the Arduino
 *  @version    1.0
 *  @author     Ersin Basaran
 *  @date       16/07/15
 *  @license    GPL v3.0
*/

#include "Kemper.h"

USING_NAMESPACE_KEMPER

USING_NAMESPACE_MIDI

#ifdef ARDUINO
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, midiOutput);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, midiInput);
#else
#include "MidiWrapper.h"
MidiOutWrapper midiOutput;
MidiInWrapper midiInput;
#endif

Kemper* instance;

const byte StompPageIds[8] = {50,51,52,53,56,58,74,75};
const byte StompActiveNumbers[8] = {3,3,3,3,3,3,2,2};

Kemper::Kemper() {
	instance = this;
	state.mode = MODE_BROWSE;

	state.currentRig = 0;
	
	state.performance = 0;
	state.slot = 0;

	for (int i = 0;i<KEMPER_STOMP_COUNT;i++) {
		state.stomps[i].info.type = 0;
	}
}

#ifndef ARDUINO
	void Kemper::setMidiPorts(int inPort, int outPort) {
		midiInput.midiIn.openPort(inPort);
		midiOutput.midiOut.openPort(outPort);
	}
#endif

void Kemper::begin() {
	midiInput.begin(KEMPER_MIDI_CHANNEL);
	midiOutput.begin(KEMPER_MIDI_CHANNEL);
	midiInput.setHandleSystemExclusive(onSysExStatic);
	midiInput.setHandleControlChange(onControlChangeStatic);
	midiInput.setHandleProgramChange(onProgramChangeStatic);
    midiInput.setHandleActiveSensing(onActiveSensingStatic);
}

void Kemper::getDelay() {
	getParam(StompPageIds[6], 0);
}

void Kemper::getReverb() {
	getParam(StompPageIds[7], 0);
}

void Kemper::getStompParameter(int stompIdx, int paramNumber) {
	getParam(StompPageIds[stompIdx], paramNumber);
}

void Kemper::getState() {
	for (int i=0;i<sizeof(StompPageIds);i++) //
	{
		getParam(StompPageIds[i], 0);
		getParam(StompPageIds[i], StompActiveNumbers[i]);
	}
}

void Kemper::read() {
	midiInput.read();
	syncState();
}

void Kemper::getTuner() {
	getParam(125, 84);
	//getParam(124, 15); // this is received through stream
}

void Kemper::getTempo() {
	getParam(4, 0);
	//getParam(4, 2); // this is received through stream
}

void Kemper::tunerOn() {
	sendControlChange(0x1F, 1);
}

void Kemper::tunerOff() {
	sendControlChange(0x1F, 0);
}

void Kemper::tapOn() {
	sendControlChange(0x1E, 1);
}

void Kemper::tapOff() {
	sendControlChange(0x1E, 0);
}

void Kemper::setRig(byte rig) {
	state.currentRig = rig;
	sendProgramChange(rig, 0);
}

void Kemper::looperRecordPlayDown() {
	setParam(0x7d, 0x58, 1);
}
void Kemper::looperRecordPlayUp() {
	setParam(0x7d, 0x58, 0);
}

void Kemper::looperReverseDown() {
	setParam(0x7d, 0x5b, 1);
}
void Kemper::looperReverseUp() {
	setParam(0x7d, 0x5b, 0);
}
void Kemper::looperHalfTimeDown() {
	setParam(0x7d, 0x5c, 1);
}
void Kemper::looperHalfTimeUp() {
	setParam(0x7d, 0x5c, 0);
}
void Kemper::looperUndoDown() {
	setParam(0x7d, 0x5d, 1);
}
void Kemper::looperUndoUp() {
	setParam(0x7d, 0x5d, 0);
}
void Kemper::looperStopEraseDown() {  // 0x5e
	setParam(0x7d, 0x59, 1);
}
void Kemper::looperStopEraseUp() {
	setParam(0x7d, 0x59, 0);
}
void Kemper::looperTriggerDown() {
	setParam(0x7d, 0x5a, 1);
}
void Kemper::looperTriggerUp() {
	setParam(0x7d, 0x5a, 0);
}

void Kemper::setParam(byte page, byte number, int val) {
	sendControlChange(0x63, page);
	sendControlChange(0x62, number);
	sendControlChange(0x06, val >> 7);
	sendControlChange(0x26, val & 127);
}

void Kemper::getRigName() {
	getStringParam(0, 1);
}

void Kemper::getStringParam(byte page, byte number) {
	sendSysEx(0x43, page, number);
}

void Kemper::getMode() {
	getParam(127, 126);
}

void Kemper::sendStream() {
	byte message[] = {0xf0, 0x00, 0x20, 0x33, 0x02, 0x7f, 0x7E, 0x00, 64, 1, 54, 4, 0xf7};
	midiOutput.sendSysEx(sizeof(message), message, true);
}


void Kemper::getParam(byte page, byte number) {
	sendSysEx(0x41, page, number);
}

void Kemper::sendControlChange(byte data1, byte data2) {
	midiOutput.send(midi::ControlChange, data1, data2, KEMPER_MIDI_CHANNEL);
}

void Kemper::sendProgramChange(byte data1, byte data2) {
	midiOutput.send(midi::ProgramChange, data1, data2, KEMPER_MIDI_CHANNEL);
}

byte message[11] = {0xf0, 0x00, 0x20, 0x33, 0x02, 0x7f, 0, 0x00, 0, 0, 0xf7};

void Kemper::sendSysEx(byte command, byte page, byte number)
{
	//byte message[] = {0xf0, 0x00, 0x20, 0x33, 0x02, 0x7f, command, 0x00, page, number, 0xf7};
	message[6] = command;
	message[8] = page;
	message[9] = number;
	midiOutput.sendSysEx(sizeof(message), message, true);
}

void Kemper::sendSysExExtended(byte command, unsigned int page, unsigned int number)
{
	byte message[] = {0xf0, 0x00, 0x20, 0x33, 0x02, 0x7f, command, 0x00, page>>12, (page>>5) & 0x7f, ((page&0x1f)<<2) | (number>>14), (number>>7) & 0x7f, number & 0x7f, 0xf7};
	midiOutput.sendSysEx(sizeof(message), message, true);
}

void Kemper::sendSysExExtended(byte command, byte b1, byte b2, byte b3, byte b4, byte b5)
{
	byte message[] = {0xf0, 0x00, 0x20, 0x33, 0x02, 0x7f, command, 0x00, b1, b2, b3, b4, b5, 0xf7};
	midiOutput.sendSysEx(sizeof(message), message, true);
}

void Kemper::onActiveSensing() {
	senseReceived = true;
}

void Kemper::toggleStomp(byte idx) {
	setParam(StompPageIds[idx], StompActiveNumbers[idx], state.stomps[idx].active?0:1);
}

void Kemper::setStompParam(int stompIdx, byte number, int val) {
	setParam(StompPageIds[stompIdx], number, val);
}

void Kemper::setPerformance(int idx, int slot /* = 0*/) {
	byte bank = 0;
	byte program = 0;
	int bankIndexes[] = {0, 25, 51, 76, 102};
	int slotOffsets[] = {0, 3, 1, 4, 2};
	for (int i=0;i<5;i++)
		if (idx>bankIndexes[i] || (idx==bankIndexes[i] && slot>=slotOffsets[i])) {
			bank = i;
			program = (idx - bankIndexes[i])*5 + (slot - slotOffsets[i]);
		}
	//debug("setPerformance");
	//debug(bank);
	//debug(program);
	sendControlChange(32, bank);
	sendProgramChange(program, 0);
}

void Kemper::onPerformanceChanged() {
	int bankIndexes[] = {0, 25, 51, 76, 102};
	int slotOffsets[] = {0, 3, 1, 4, 2};
	state.performance = (state.program + slotOffsets[state.bank])/5 + bankIndexes[state.bank];
	state.slot = state.program - 5*(state.performance - bankIndexes[state.bank]) + slotOffsets[state.bank];
	//debug("Calculated performance/slot");
	//debug(state.performance);
	//debug(state.slot);
	
}

void Kemper::onControlChange(byte channel, byte number, byte value) {
	if (number == 0x20) {
		if (state.mode == MODE_PERFORM) {
			state.bank = value;
			debug("Control change");
			debug(value);
			onPerformanceChanged();
		}
	}

}

void Kemper::onProgramChange(byte channel, byte number) {
	if (state.mode == MODE_PERFORM) {
		state.program = number;
		onPerformanceChanged();
	}
}

void Kemper::onSysEx(byte* buffer, unsigned int size) {
	data = midiInput.getSysExArray();
	int messageLength = midiInput.getSysExArrayLength();

	if (messageLength >= 12)
	{
		byte page, number;
		int value;
		int stompId = -1;
		page = data[8];
		number = data[9];

		switch (data[6])
		{
			case 1: //get param
				value = (data[10]<<7) | data[11];
				for (int i=0;i<sizeof(StompPageIds);i++) {
					if (StompPageIds[i] == page) {
						stompId = i;
						break;
					}
				}
				if (stompId>=0) 
				{
					if (number == 0)
					{
						if ((state.stomps[stompId].info.type!=0 || state.stomps[stompId].info.type != value)) {
							//getStompInfo(stompId, value, &state.stomps[stompId].info);
							loadStompInfo(&state.stomps[stompId].info, stompId, value);
							state.stomps[stompId].dirty = 1;
						} else if (value == 0) {
						}
					}
					if (number == StompActiveNumbers[stompId]) {
						state.stomps[stompId].active = value?1:0;
						state.stomps[stompId].dirty = 1;
					}
					if (stompId == parameter.stompIdx) {
						if (updateStompParameterValue(&parameter, value)) {
							loadStompParameters(&parameter);
							state.parameterState++;
						}
					}
					if (stompId == lastStompParam[0] && number == lastStompParam[1]) {
						//Serial.print("Parameter received: ");
						//Serial.print(stompId);
						//Serial.print(" ");
						//Serial.print(number);
						//Serial.print(" ");
						//Serial.println(value);
						lastStompParam[2] = value;
					}
				}
				if (page == 4) {
					if (number == 0)
						state.tempo = value;
					else if (number == 2)
						state.tempoEnabled = value>0;
				}
				if (page == 124) {
					if (number == 0) {
						state.tempoLed = value?true:false;
					}
					if (number == 15) {
						state.tune = value;
					}
				}
				if (page == 125) {
					if (number == 84) {
						state.key = value;
					}
				}
				if (page == 127 && number == 126) {
					if (state.mode != data[11])
					{
						state.mode = data[11];
						//debug("MODE CHANGED");
						//debug(state.mode);
					}
				}
				break;
			case 3: //get string param
				for (int i=10;i<messageLength-1 && i-10<20-1; i++) {
					state.rigName[i-10] = (char)data[i];
				}
				state.rigName[min(messageLength-1-10, 19)] = 0;
				break;
			case 7:
				int i=0;
				
				int slotId = (int)data[12];
				if (slotId>=0 && slotId<=5)
				{
					for (i=13;i<messageLength-2; i++) {
						state.performanceNames[slotId][i-13] = (char)data[i];
					}
					state.performanceNames[slotId][i-13] = 0;
				}
				else 
				{
					debug(F("Error in performance name slotId: "));
					debug(slotId);
					debug2(F("\n\nError in performance name slotId: "));
				}
		}
	}
}

void Kemper::onSysExStatic(byte* buffer, unsigned int size) {
	instance->onSysEx(buffer, size);
}
void Kemper::onControlChangeStatic(byte channel, byte number, byte value) {
	instance->onControlChange(channel, number, value);
}
void Kemper::onProgramChangeStatic(byte channel, byte number) {
	instance->onProgramChange(channel, number);
}
void Kemper::onActiveSensingStatic() {
	//instance->onActiveSensing();
}

void Kemper::syncState() {
	static unsigned long lastTimeStream = -10000;
	static unsigned long firstRunTime = 1;
	static unsigned long lastDelay = 0;
	static unsigned long lastReverb = 0;
	static unsigned long lastTempo = 0;
	static unsigned long lastTuner = 0;
	static unsigned long lastMode = 0;

	if (millis() - lastTimeStream > 5000) { 
		sendStream();
		lastTimeStream = millis();
	}
	if (firstRunTime && millis() - firstRunTime > 400) {
		//setRig(0);
		getState();
		firstRunTime = 0;
	}

	if (millis() - lastDelay > 200) {
		getDelay();
		lastDelay = millis();
	} 
	if (millis() - lastReverb> 220) {
		getReverb();
		lastReverb = millis();
	}
	if (millis() - lastTuner > 24) {
		getTuner();
		lastTuner = millis();
	}

	if (millis() - lastTempo > 53) {
		getTempo();
		lastTempo = millis();
	}
	if (millis() - lastMode > 1017) {
		getMode();
		lastMode = millis();
	} 

	static unsigned long lastPerfNameTime = 0;
	static int lastPerfIdx = 0;

	if (state.mode == MODE_PERFORM)
	{
		for (int i = 0;i<6;i++)
		{
			if (state.performanceNames[i][0] == 0 && millis() - lastPerfNameTime > 130) 
			{
				sendSysExExtended(0x47, 0, 0, 1, 0, i);
				lastPerfIdx = lastPerfIdx%6;
				lastPerfNameTime = millis();
			}
		}
	}

	static unsigned long lastBrowseNameTime = 0;

	if (state.mode == MODE_BROWSE)
	{
		if (millis() - lastBrowseNameTime > 500 && state.rigName[0] == 0) {
			getRigName();
			lastBrowseNameTime = millis();
		}
	}

	//digitalWrite(13, state.tempoLed?HIGH:LOW);
}

