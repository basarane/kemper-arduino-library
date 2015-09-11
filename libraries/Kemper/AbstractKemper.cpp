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
}

void AbstractKemper::loadPartialParam(int stompIdx) {
	parameter.isActive = true;
	parameter.currentParam = 0;
	parameter.currentOption = 0; //@ersin - burasi kemper'den gelecek
	loadStompParameters(&parameter, &state.stomps[stompIdx].info);
	state.parameterState++;
}

void AbstractKemper::movePartialParam(int paramOffset, int optionOffset) {
	int curParamVal = parameter.currentParam;
	int curOptionVal = 0;

	parameter.currentParam += paramOffset;
	parameter.currentParam = max(parameter.currentParam, 0);
	parameter.currentParam = min(parameter.currentParam, parameter.totalParamCount-1);
	
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
	if (optionValue != parameter.currentOption) {
		setStompParam(parameter.stompIdx, parameter.params[parameter.currentParam - parameter.startParamIndex].number, optionValue);
	}
}