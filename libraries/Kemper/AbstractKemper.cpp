/*!
 *  @file       Kemper.cpp
 *  Project     Arduino Kemper MIDI Library
 *  @brief      Kemper MIDI Library for the Arduino
 *  @version    1.0
 *  @author     Ersin Basaran
 *  @date       16/07/15
 *  @license    GPL v3.0
*/

#include "AbstractKemper.h"

USING_NAMESPACE_KEMPER

AbstractKemper::AbstractKemper() {
	state.mode = MODE_BROWSE;
	state.performanceNames[0][0] = 0;
	state.performanceNames[1][0] = 0;
	state.performanceNames[2][0] = 0;
	state.performanceNames[3][0] = 0;
	state.performanceNames[4][0] = 0;
	state.performanceNames[5][0] = 0;
	memset(lastStompParam, -1, sizeof(lastStompParam));
	lastStompParamTime = 0;
	state.looperState.state = LOOPER_STATE_EMPTY;
	state.looperState.isHalfTime = false;
	state.looperState.isReversed = false;
	state.looperState.erasePressCount = 0;
	state.looperState.atLestOneLayer = false;

	parameter.isActive = false;
}

void AbstractKemper::loadPartialParam(int stompIdx) {
	parameter.isActive = true;
	parameter.currentParam = 0;
	parameter.currentOption = 0; //@ersin - burasi kemper'den gelecek
	parameter.stompIdx = stompIdx;
	loadStompParameters(&parameter, &state.stomps[stompIdx].info);
	state.parameterState++;
}

void AbstractKemper::movePartialParam(int paramOffset, int optionOffset) {
	int curParamVal = parameter.currentParam;
	int curOptionVal = 0;

	parameter.currentParam += paramOffset;
	parameter.currentParam = max(parameter.currentParam, 0);
	parameter.currentParam = min(parameter.currentParam, parameter.totalParamCount-1);
	
	if (paramOffset==0)
	{
		if (parameter.totalOptionCount>0) {
			curOptionVal = parameter.currentOption;
			curOptionVal += optionOffset;
			curOptionVal = max(curOptionVal, 0);
			curOptionVal = min(curOptionVal, parameter.totalOptionCount-1);
			curOptionVal = getOptionValue(&parameter, curOptionVal);
		} else {
			curOptionVal = parameter.currentValue;
			curOptionVal += optionOffset * (1<<9);
			curOptionVal = max(curOptionVal, 0);
			curOptionVal = min(curOptionVal, (1<<14)-1);
		}
	}

	if (curParamVal!=parameter.currentParam) {
		loadStompParameters(&parameter);
	}
	if (curOptionVal!=parameter.currentOption && paramOffset==0) {
		setStompParam(parameter.stompIdx, parameter.params[parameter.currentParam - parameter.startParamIndex].number, curOptionVal);
	}
	if (curParamVal!=parameter.currentParam)
		state.parameterState++;
}

void AbstractKemper::setPartialParamValue(float value) {
	int optionValue = 0;
	if (parameter.totalOptionCount > 0) {
		optionValue = floor(value * (parameter.totalOptionCount-0.001));
		optionValue = getOptionValue(&parameter, optionValue);
	} else {
		optionValue = floor(value * ((1<<14)-1));
	}
	if ((parameter.totalOptionCount>0 && optionValue != parameter.currentOption) || (parameter.totalOptionCount==0 && optionValue!=parameter.currentValue)) {
		setStompParam(parameter.stompIdx, parameter.params[parameter.currentParam - parameter.startParamIndex].number, optionValue);
	}
}

void AbstractKemper::looperRecordPlayDown() {
	state.looperState.erasePressCount = 0;
	if (state.looperState.state == LOOPER_STATE_EMPTY || state.looperState.state == LOOPER_STATE_ERASED) {
		state.looperState.state = LOOPER_STATE_RECORDING;
		state.looperState.atLestOneLayer = false;
	}
	else if (state.looperState.state == LOOPER_STATE_PLAYBACK) {
		state.looperState.state = LOOPER_STATE_OVERDUB;
		state.looperState.atLestOneLayer = true;
	}
	else if (state.looperState.state == LOOPER_STATE_RECORDING || state.looperState.state == LOOPER_STATE_OVERDUB || state.looperState.state == LOOPER_STATE_STOPPED)
		state.looperState.state = LOOPER_STATE_PLAYBACK;
	state.looperState.recordPressTime = millis();
}
void AbstractKemper::looperRecordPlayUp() {
	if (state.looperState.state == LOOPER_STATE_RECORDING || state.looperState.state == LOOPER_STATE_OVERDUB) {
		if (millis() - state.looperState.recordPressTime > 500) {
			state.looperState.state = LOOPER_STATE_PLAYBACK;
		}
	}
}
void AbstractKemper::looperReverseDown() {
	state.looperState.isReversed = !state.looperState.isReversed;
}
void AbstractKemper::looperReverseUp() {
}
void AbstractKemper::looperHalfTimeDown() {
	state.looperState.isHalfTime = !state.looperState.isHalfTime;
}
void AbstractKemper::looperHalfTimeUp() {
}
void AbstractKemper::looperUndoDown() {
	state.looperState.erasePressCount = 0;
	if (state.looperState.state == LOOPER_STATE_ERASED)
		state.looperState.state = LOOPER_STATE_PLAYBACK;
	else if (state.looperState.state == LOOPER_STATE_RECORDING)
		state.looperState.state = LOOPER_STATE_RECORDING_UNDO;
	else if (state.looperState.state == LOOPER_STATE_RECORDING_UNDO)
		state.looperState.state = LOOPER_STATE_PLAYBACK;
	else if (state.looperState.state == LOOPER_STATE_PLAYBACK && !state.looperState.atLestOneLayer)
		state.looperState.state = LOOPER_STATE_RECORDING_UNDO;
}
void AbstractKemper::looperUndoUp() {

}
void AbstractKemper::looperStopEraseDown() {
	state.looperState.erasePressTime = millis();
	state.looperState.erasePressCount++;
	if (state.looperState.state != LOOPER_STATE_ERASED && state.looperState.state != LOOPER_STATE_EMPTY)
		state.looperState.state = LOOPER_STATE_STOPPED;
	if (state.looperState.erasePressCount >= 3 && state.looperState.state != LOOPER_STATE_EMPTY)
		state.looperState.state = LOOPER_STATE_ERASED;
}
void AbstractKemper::looperStopEraseUp() {
	if (millis() - state.looperState.erasePressTime >= 2000) {
		state.looperState.state = LOOPER_STATE_ERASED;
	}
}
void AbstractKemper::looperTriggerDown() {
	state.looperState.erasePressCount = 0;
}
void AbstractKemper::looperTriggerUp() {

}
