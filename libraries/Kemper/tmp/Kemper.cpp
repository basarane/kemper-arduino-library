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

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, midiOutput);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, midiInput);

Kemper* instance;

const byte StompPageIds[8] = {50,51,52,53,56,58,74,75};
const byte StompActiveNumbers[8] = {3,3,3,3,3,3,2,2};

Kemper::Kemper() {
	instance = this;
	state.mode = MODE_BROWSE;
}

void Kemper::begin() {
	midiInput.begin(KEMPER_MIDI_CHANNEL);
	midiOutput.begin(KEMPER_MIDI_CHANNEL);
	midiInput.setHandleSystemExclusive(onSysExStatic);
	midiInput.setHandleControlChange(onControlChangeStatic);
	midiInput.setHandleProgramChange(onProgramChangeStatic);
    midiInput.setHandleActiveSensing(onActiveSensingStatic);

	pinMode(13, OUTPUT);
}

void Kemper::getDelay() {
	getParam(StompPageIds[6], 0);
}

void Kemper::getReverb() {
	getParam(StompPageIds[7], 0);
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

void Kemper::sendSysEx(byte command, byte page, byte number)
{
	byte message[] = {0xf0, 0x00, 0x20, 0x33, 0x02, 0x7f, command, 0x00, page, number, 0xf7};
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
	debug("Calculated performance/slot");
	debug(state.performance);
	debug(state.slot);
	
}

void Kemper::onControlChange(byte channel, byte number, byte value) {
	if (number == 0x20) {
		if (state.mode == MODE_PERFORM) {
			state.bank = value;
			debug("Control change");
			onPerformanceChanged();
		}
	}

#ifdef KEMPER_DEBUG
	Serial.print("\nCONTROLCHANGE: ");
	Serial.print(channel);
	Serial.print(" ");
	Serial.print(number);
	Serial.print(" ");
	Serial.print(value);
	Serial.print(" Bank: ");
	Serial.print(state.bank);
	Serial.print(" PRogram: ");
	Serial.print(state.program);
#endif
}

void Kemper::onProgramChange(byte channel, byte number) {
	if (state.mode == MODE_PERFORM) {
		state.program = number;
		debug("PRogram change");
		onPerformanceChanged();
	}
#ifdef KEMPER_DEBUG
	Serial.print("\nPROGRAM CHANGE: ");
	Serial.print(channel);
	Serial.print(" ");
	Serial.print(number);
	Serial.print(" Bank: ");
	Serial.print(state.bank);
	Serial.print(" PRogram: ");
	Serial.print(state.program);
#endif
}

void Kemper::onSysEx(byte* buffer, unsigned int size) {
	data = midiInput.getSysExArray();
	int messageLength = midiInput.getSysExArrayLength();
#ifdef KEMPER_DEBUG
	//Serial.write(0xF1);
	//Serial.write(data, messageLength);
	//Serial.write(0xFF);
	//Serial.flush();
	if (data[6]==1 && data[8] == 127)
	{
		Serial.print("\nSYSEXRECEIVED (");
		Serial.print(messageLength);
		Serial.print("bytes) :");
		for (int i=0;i<messageLength;i++) {
			Serial.print(data[i]);
			Serial.print(" ");
		}
		if (data[6] == 7 || data[6] == 3)
		{
			for (int i=data[6]==7?13:10;i<messageLength-2;i++) {
				Serial.write(data[i]);
			}
		}
	}
#endif
	if (messageLength >= 12)
	{
		byte page, number;
		int value;
		String stringValue = "";
		int stompId = -1;
		page = data[8];
		number = data[9];

		switch (data[6])
		{
			case 1: //get param
				value = (data[10]<<7) | data[11];
				//snprintf(lineBuffer, sizeof(lineBuffer), "%d,%d => %d", page, number, value);
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
							getStompInfo(stompId, value, &state.stomps[stompId].info);
							state.stomps[stompId].dirty = 1;
						} else if (value == 0) {
						}
					}
					if (number == StompActiveNumbers[stompId]) {
						state.stomps[stompId].active = value?1:0;
						state.stomps[stompId].dirty = 1;
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
						debug("MODE CHANGED");
						debug(state.mode);
					}
				}
				break;
			case 3: //get string param
				for (int i=10;i<messageLength-1; i++) {
					stringValue += (char)data[i];
				}
				//Serial.println("STRING VALUE: " + stringValue); 
				state.rigName = stringValue;
				stringValue = "";
				break;
			case 7:
				//break;
				for (int i=13;i<messageLength-2; i++) {
					stringValue += (char)data[i];
				}
				state.performanceNames[(int)data[12]] = stringValue;
				debug("Performance name");
				debug((int)data[12]);
				debug(stringValue);
				
				stringValue = "";
				for (int i=0;i<messageLength-1; i++) {
					stringValue += " ";
					stringValue += (int)data[i];
				}
				debug(stringValue);
				
				stringValue = "";
#ifdef KEMPER_DEBUG
				Serial.print("\nPERFORMANCE NAMES\n");
				Serial.println(state.performanceNames[0]);
				Serial.println(state.performanceNames[1]);
				Serial.println(state.performanceNames[2]);
				Serial.println(state.performanceNames[3]);
				Serial.println(state.performanceNames[4]);
				Serial.println(state.performanceNames[5]);

#endif
		}
		/*
		for (int i=0;i<midiInput.getSysExArrayLength();i++) {
			snprintf(lineBuffer, 6, "%d,", data[i]);
			Serial.print(lineBuffer);  
		}
		Serial.println(" OK");
		*/
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
	instance->onActiveSensing();
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
		//Serial.println("STREAM READ");
	}
	if (firstRunTime && millis() - firstRunTime > 400) {
		//Serial.println("SET RIG");
		setRig(0);
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
	if (millis() - lastTuner > 107) {
		getTuner();
		lastTuner = millis();
	}
	/*
	*/
	if (millis() - lastTempo > 53) {
		getTempo();
		lastTempo = millis();
	}
	if (millis() - lastMode > 1017) {
		getMode();
		lastMode = millis();
	}
	static unsigned long lastPerfNameTime = 0;

	if (state.mode == MODE_PERFORM)
	{
		for (int i = 0;i<6;i++)
		{
			if (state.performanceNames[i].length() == 0 && millis() - lastPerfNameTime > 100)
			{
				//getStringParam(7, 0);
				debug("SendExtendedString");
				sendSysExExtended(0x47, 0, 0, 1, 0, i);
				lastPerfNameTime = millis();
			}
		}
	}
	digitalWrite(13, state.tempoLed?HIGH:LOW);
}

