/*!
 *  @file       Kemper.cpp
 *  Project     Arduino Kemper MIDI Library
 *  @brief      Kemper MIDI Library for the Arduino
 *  @version    1.0
 *  @author     Ersin Basaran
 *  @date       16/07/15
 *  @license    GPL v3.0
*/

#include "KemperMock.h"

USING_NAMESPACE_KEMPER

KemperMock::KemperMock() {
	state.mode = MODE_BROWSE;
	state.currentRig = 0;
	state.tempoEnabled = false;

	state.performance = 0;
	state.slot = 0;

	//srand (time(NULL));
	srand (0);

	for (int i = 0;i<KEMPER_STOMP_COUNT;i++) {
		state.stomps[i].info.type = 0;
	}

	for (int t = 0;t<2;t++) {
		for (int i = 0;i<RIG_COUNT*5;i++) {
			int rIdx = 0;
			for (int j = 0;j<6;j++) {
				if (rand()%10<8) {
					rIdx = rand() % (AllStompsCount - 8);
					defaultStompStates[t][i][j].active = rand()%10 < 7;
					defaultStompStates[t][i][j].info = (AllStomps[rIdx]);
				}
				else {
					defaultStompStates[t][i][j].active = false;
					defaultStompStates[t][i][j].info.type = 0;
				}
			}
			rIdx = (AllStompsCount - 8) + (rand() % 3);
			defaultStompStates[t][i][6].active = rand()%10<8;
			defaultStompStates[t][i][6].info = (AllStomps[rIdx]);

			rIdx = (AllStompsCount - 5) + (rand() % 5);
			defaultStompStates[t][i][7].active = rand()%10<8;
			defaultStompStates[t][i][7].info = (AllStomps[rIdx]);
		}
	}
	state.tune = 0x2000;
}

void KemperMock::read() {
	state.tempo = rand()%0x3FFF;
	state.tempoLed = millis()%500<250;
	state.key = rand()%60;
	//tune = rand()%0x3FFF;
	state.tune += (rand()%401) - 200;
}

void KemperMock::tunerOn() {
	lastMode = state.mode;
	state.mode = MODE_TUNER;
}

void KemperMock::tunerOff() {
	state.mode = lastMode;
}

void KemperMock::tapOn() {
	state.tempoEnabled = true;
}

void KemperMock::tapOff() {
}

void KemperMock::setRig(byte rig) {
	state.currentRig = rig;
	sprintf(state.rigName, "RIG #%d", (int)(state.currentRig+1));
	for (int i=0;i<KEMPER_STOMP_COUNT;i++) {
		state.stomps[i].active = defaultStompStates[0][state.currentRig][i].active;
		state.stomps[i].info = defaultStompStates[0][state.currentRig][i].info;
	}
}

void KemperMock::looperRecordPlayDown() {
}
void KemperMock::looperRecordPlayUp() {
}

void KemperMock::looperReverseDown() {
}
void KemperMock::looperReverseUp() {
}

void KemperMock::looperHalfTimeDown() {
}
void KemperMock::looperHalfTimeUp() {
}
void KemperMock::looperUndoDown() {
}
void KemperMock::looperUndoUp() {
}
void KemperMock::looperStopEraseDown() {
}
void KemperMock::looperStopEraseUp() {
}
void KemperMock::looperTriggerDown() {
}
void KemperMock::looperTriggerUp() {
}

void KemperMock::toggleStomp(byte idx) {
	state.stomps[idx].active = !state.stomps[idx].active;
}

void KemperMock::setPerformance(int idx, int slot) {
	state.performance = idx;
	state.slot = slot;

	sprintf(state.performanceNames[0], "(%d) PERFORMANCE #%d", (state.performance+1), (state.performance+1));
	sprintf(state.performanceNames[1], "SLOT %d", 1);
	sprintf(state.performanceNames[2], "SLOT %d", 2);
	sprintf(state.performanceNames[3], "SLOT %d", 3);
	sprintf(state.performanceNames[4], "SLOT %d", 4);
	sprintf(state.performanceNames[5], "SLOT %d", 5);

	for (int i=0;i<KEMPER_STOMP_COUNT;i++) {
		state.stomps[i].active = defaultStompStates[1][state.performance*5+slot][i].active;
		state.stomps[i].info = defaultStompStates[1][state.performance*5+slot][i].info;
	}

}

void KemperMock::sendControlChange(byte data1, byte data2) {
}