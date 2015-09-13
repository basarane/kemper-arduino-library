
#include "KemperRemote.h"

USING_NAMESPACE_KEMPER

int KEMPER_NAMESPACE::ExpressionPedalModes[] = {CC_WAH, CC_PITCH, CC_VOLUME};

KemperRemote::KemperRemote(AbstractKemper *_kemper) { //
	kemper = _kemper;
	state.state = REMOTE_STATE_NORMAL;
	state.currentPage = 0;
	currentSwitch = -1;
	for (int i=0;i<SWITCH_COUNT;i++)
		switchStates[i] = false;
	switchDownStart = 0;
	rigAssignRig = -1;
	rigAssignSwitch = -1;
	looperState = LOOPER_STATE_NONE;

	state.currentPerformance = -1;
	state.currentSlot = -1;
	saveUpDown = 0;

	for (int i=0;i<RIG_COUNT;i++)
		rigMap[i] = i;

	for (int i=0;i<SWITCH_RIG_COUNT;i++)
		for (int j=0;j<SWITCH_STOMP_COUNT;j++) {
			stompAssignment[i][j] = 0;
		}
	for (int k = 0; k<PERFORM_SLOT_COUNT; k++) {
		for (int j = 0; j < SWITCH_STOMP_COUNT; j++) {
			stompAssignmentPerform[k][j] = 0;
		}
	}
	for (int j=0;j<SWITCH_STOMP_COUNT;j++)
		currentStompAssignment[j] = 0;

	EEPROM.get(sizeof(stompAssignment)*BROWSE_PAGE_COUNT + sizeof(stompAssignmentPerform)*RIG_COUNT, rigMap);

	/*
	//@ersin - add EEPROM save
	EEPROM.get( 0, stompAssignment );
	EEPROM.get( sizeof(stompAssignment), stompAssignmentPerform );

	for (int i=0;i<RIG_COUNT;i++)
		for (int j=0;j<SWITCH_STOMP_COUNT;j++) {
			if (stompAssignment[i][j] == 255)
				stompAssignment[i][j] = 0;
			for (int k=0;k<PERFORM_SLOT_COUNT;k++) {
				if (stompAssignmentPerform[i][k][j] == 255)
					stompAssignmentPerform[i][k][j] = 0;
			}
		}
		*/
	for (int i=0;i<RIG_COUNT;i++)
		if (rigMap[i] == 255)
			rigMap[i] = i;

	for (int j=0;j<SWITCH_STOMP_COUNT;j++)
		currentStompAssignment[j] = 0;
	for (int j=0;j<EXPRESSION_PEDAL_COUNT;j++) {
		expPedals[j].minValue = 1024;
		expPedals[j].maxValue = -1;
	}

	state.isSaved = true;
	expPedals[0].begin(3);

	memset(parameterBuffer, -1, sizeof(parameterBuffer));
	nextParameters = parameterBuffer;
	currentParameters = 0;
}

void KemperRemote::read() {
	if (state.state == REMOTE_STATE_STOMP_PARAMETER_LOAD || state.state == REMOTE_STATE_STOMP_PARAMETER_POST_LOAD) {
		int stompIdx = kemper->lastStompParam[0];
		int paramNumber = kemper->lastStompParam[1];
		int paramValue = kemper->lastStompParam[2];
		int(*stompParam)[KEMPER_STOMP_COUNT][MAX_KEMPER_PARAM_LENGTH][2];
		stompParam = &oldStompParameters;
		if (state.state == REMOTE_STATE_STOMP_PARAMETER_POST_LOAD) {
			stompParam = &newStompParameters;
		}
		bool nextParam = false;
		if (stompIdx >= 0) {
			if (paramValue >= 0) {
				nextParam = true;
				for (int i = 0; i < MAX_KEMPER_PARAM_LENGTH;i++)
					if ((*stompParam)[stompIdx][i][0] == paramNumber) {
						(*stompParam)[stompIdx][i][1] = paramValue;
						break;
					}
			}
		} 
		else
			nextParam = true;
		if (nextParam) {
			bool paramFound = false;
			for (int i = 0; i < KEMPER_STOMP_COUNT && !paramFound; i++) {
				StompInfo *info = &kemper->state.stomps[i].info;
				for (int j = 0; j < info->paramCount; j++) {
					if ((*stompParam)[i][j][0] >= 0 && (*stompParam)[i][j][1] < 0) {
						paramFound = true;
						kemper->lastStompParam[0] = i;
						kemper->lastStompParam[1] = (*stompParam)[i][j][0];
						kemper->lastStompParam[2] = -1;
						kemper->getStompParameter(i, (*stompParam)[i][j][0]);
						break;
					}
				}
			}
			if (!paramFound) {
				kemper->lastStompParam[0] = -1;
				kemper->lastStompParam[1] = -1;
				kemper->lastStompParam[2] = -1;
				if (state.state == REMOTE_STATE_STOMP_PARAMETER_POST_LOAD) {
					byte changeCount = 0;
					for (int i = 0; i < KEMPER_STOMP_COUNT; i++) {
						for (int j = 0; j < MAX_KEMPER_PARAM_LENGTH; j++) {
							if (oldStompParameters[i][j][1] >= 0 && oldStompParameters[i][j][1] != newStompParameters[i][j][1]) {
								changeCount++;
							}
						}
					}
					if (changeCount == 0) {
						debug("No change detected, canceling parameter mode");
					} else if (changeCount * 6 + 3 > PARAMETER_BUFFER_SIZE - (nextParameters - parameterBuffer)) {
						debug("Buffer is full! Cannot save parameters");
					}
					else
					{
						if (currentParameters) { // the rig already has stomp parameters #7
							byte* p = currentParameters;
							p += *(p + 2) * 6 + 3; // move to next position
							while (p<parameterBuffer + PARAMETER_BUFFER_SIZE && (*p!=0xff || *(p+1)!=0xff)) {
								int blockSize = *(p + 2) * 6 + 3;
								memmove(currentParameters, p, blockSize);
								currentParameters += blockSize;
								p += blockSize; // move to next position
							}
							memset(currentParameters, -1, PARAMETER_BUFFER_SIZE - (currentParameters - parameterBuffer));
							nextParameters = currentParameters;
						}
						currentParameters = nextParameters;
						if (kemper->state.mode == MODE_PERFORM) {
							*nextParameters++ = kemper->state.performance;
							*nextParameters++ = kemper->state.slot;
						}
						else {
							*nextParameters++ = 0xFF;
							*nextParameters++ = kemper->state.currentRig;
						}
						*nextParameters++ = changeCount;
						for (int i = 0; i < KEMPER_STOMP_COUNT; i++) {
							for (int j = 0; j < MAX_KEMPER_PARAM_LENGTH; j++) {
								if (oldStompParameters[i][j][1] >= 0 && oldStompParameters[i][j][1] != newStompParameters[i][j][1]) {
									*nextParameters++ = i;
									*nextParameters++ = oldStompParameters[i][j][0];
									*nextParameters++ = oldStompParameters[i][j][1] & 0xFF;
									*nextParameters++ = oldStompParameters[i][j][1] >> 8;
									*nextParameters++ = newStompParameters[i][j][1] & 0xFF;
									*nextParameters++ = newStompParameters[i][j][1] >> 8;
								}
							}
						}
					}
					state.state = REMOTE_STATE_NORMAL;
				} 
				else
					state.state = REMOTE_STATE_STOMP_PARAMETER;
			}
		}
	}


	static unsigned long ledUpdate = 0;
	if (millis() - ledUpdate < 10)
		return;
	ledUpdate = millis();

	static unsigned long lastParamTime = 0;
	if (millis() - lastParamTime>25 && kemper->parameter.isActive && state.state!=REMOTE_STATE_STOMP_PARAMETER_LOAD) {
		kemper->getStompParameter(kemper->parameter.stompIdx, kemper->parameter.params[kemper->parameter.currentParam - kemper->parameter.startParamIndex].number);
		lastParamTime = millis();
	}

	for (int i=0;i<EXPRESSION_PEDAL_COUNT;i++) {
		expPedals[i].read();
		static unsigned long lastCalibrateDebug = 0;
		static unsigned long pedalFirstNonZeroTimes[EXPRESSION_PEDAL_COUNT];
		static unsigned long pedalFirstZeroTimes[EXPRESSION_PEDAL_COUNT];
		if (millis() > 1000 && expPedals[i].value > 0 && !expPedals[i].isCalibrated() && state.state != REMOTE_STATE_EXPRESSION_CALIBRATE) {
			if (pedalFirstNonZeroTimes[i] == 0)
				pedalFirstNonZeroTimes[i] = millis();
			else if (millis() - pedalFirstNonZeroTimes[i] > 2000) {
				state.previousState = state.state;
				state.state = REMOTE_STATE_EXPRESSION_CALIBRATE;
				calibratePedalId = i;
			}
		}
		if (millis() - lastCalibrateDebug > 1000) {
			//debug("Pedal read");
			//debug(expPedals[i].value);
			//debug(expPedals[i].minValue);
			//debug(expPedals[i].maxValue);
			lastCalibrateDebug = millis();
		}
		if (expPedals[i].value == 0) {
			if (state.state == REMOTE_STATE_EXPRESSION_CALIBRATE && calibratePedalId == i && millis() - pedalFirstZeroTimes[i] > 100 && pedalFirstZeroTimes[i]>0)
				state.state = state.previousState;
			pedalFirstNonZeroTimes[i] = 0;
			if (pedalFirstZeroTimes[i] == 0)
				pedalFirstZeroTimes[i] = millis();
		} else {
			pedalFirstZeroTimes[i] = 0;
		}
		if (state.state == REMOTE_STATE_EXPRESSION_CALIBRATE) {
			expPedals[i].calibrate();
		}
		if (state.state == REMOTE_STATE_STOMP_PARAMETER && state.parameterState!=REMOTE_PARAMETER_STATE_PARAMETER && expPedals[i].isCalibrated() && i==0) { //only first exp pedal
			static unsigned int lastExpValue = 0;
			if (abs((int)(expPedals[i].calibratedValue() - lastExpValue)) > 10) { // eliminate some noise
				lastExpValue = expPedals[i].calibratedValue();
				float t = (float)expPedals[i].calibratedValue() / 1023;
				kemper->setPartialParamValue(t);
			}
		}
		static unsigned long lastParamSent = 0;
		if ((state.state == REMOTE_STATE_NORMAL || state.state == REMOTE_STATE_LOOPER) && currentParameters!=0 && i==0 && millis() - lastParamSent > 50) {
			lastParamSent = millis();
			byte* p;
			p = (currentParameters + 2);
			byte count = *p++;
			float t = (float)expPedals[i].calibratedValue() / 1023;
			for (int i = 0; i < count; i++) {
				byte stompId = *p++;
				byte paramNumber = *p++;
				int value1 = *p | ((*(p+1)) << 8);
				p += 2;
				int value2 = *p | ((*(p+1)) << 8);
				p += 2;
				int value = (1 - t)*value1 + t*value2;
				kemper->setStompParam(stompId, paramNumber, value);
			}
		}
	}
	static int lastKemperMode = -1;
	if (kemper->state.mode == MODE_BROWSE)
	{
		if (rigMap[currentRig] != kemper->state.currentRig || lastKemperMode!=MODE_BROWSE) {
			currentRig = getRigIndex(kemper->state.currentRig);
			updateCurrentParameter(0xFF, kemper->state.currentRig);
			EEPROM.get((currentRig/SWITCH_RIG_COUNT)*sizeof(stompAssignment), stompAssignment);
			memcpy(currentStompAssignment, stompAssignment[currentRig%SWITCH_RIG_COUNT], SWITCH_STOMP_COUNT);
		}
	}
	if (kemper->state.mode == MODE_PERFORM) {
		if (state.currentPerformance != kemper->state.performance) {
			EEPROM.get(BROWSE_PAGE_COUNT*sizeof(stompAssignment) + (kemper->state.performance)*sizeof(stompAssignmentPerform), stompAssignmentPerform);
		}
		if (state.currentPerformance != kemper->state.performance || state.currentSlot != kemper->state.slot || lastKemperMode!=MODE_PERFORM) {
			state.currentPerformance = kemper->state.performance;
			state.currentSlot = kemper->state.slot;
			updateCurrentParameter(state.currentPerformance, state.currentSlot);
			memcpy(currentStompAssignment, stompAssignmentPerform[state.currentSlot], SWITCH_STOMP_COUNT);
		}
	}

	lastKemperMode = kemper->state.mode;
	refreshStompAssignment();
	checkSwitchDownLong();
	checkUpDownScroll();
	checkStompChanges();
	updateLeds(); 

	for (int i = 0; i < EXPRESSION_PEDAL_COUNT;i++)
	if (expPedals[i].isCalibrated() && state.state!=REMOTE_STATE_EXPRESSION_CALIBRATE && expPedals[i].isChanged(8))
	{
		if (i!=0 || state.state!=REMOTE_STATE_STOMP_PARAMETER && currentParameters==0) // first exp. pedal is for parameter update
			kemper->sendControlChange(expPedals[i].mode, (expPedals[i].calibratedValue())/8);
	}
}

void KemperRemote::updateCurrentParameter(byte perf, byte slot) {
	byte* p = parameterBuffer;
	currentParameters = 0;
	while (p<parameterBuffer + PARAMETER_BUFFER_SIZE)
	{
		if (*p == perf && *(p + 1) == slot) {
			currentParameters = p;
			break;
		}
		if (*p == 0xff && *(p + 1) == 0xff)
			break;
		p = p + 3 + *(p + 2) * 6;
	}
}

byte KemperRemote::getRigIndex(byte rig) {
	for (int i=0;i<RIG_COUNT;i++)
		if (rigMap[i] == rig)
			return i;
	return 0;
}

void KemperRemote::refreshStompAssignment() {
	byte availableStomps = 0;
	for (int i=0;i<KEMPER_STOMP_COUNT;i++)
		if (kemper->state.stomps[i].info.type>0)
			availableStomps = availableStomps | (1<<i);
	byte* assignment = 0;
	if (kemper->state.mode == MODE_BROWSE) {
		assignment = stompAssignment[currentRig];
	} else if (kemper->state.mode == MODE_PERFORM) {
		assignment = stompAssignmentPerform[state.currentSlot];
	}
	for (int i = 0;i<SWITCH_STOMP_COUNT;i++) {
		if (assignment && assignment[i]==0) {
			currentStompAssignment[i] = 0;
			byte tmp = availableStomps;
			byte a = 0;
			while (!(tmp & 1) && tmp > 0) {
				a++;
				tmp = tmp>>1;
			}
			if (tmp > 0) {
				availableStomps = availableStomps & (0xFF - 1<<a);
				currentStompAssignment[i] |= 1<<a;
			}
		}
	}
}

void KemperRemote::onStompDown(int switchIdx) {
	byte assignment = currentStompAssignment[switchIdx];
	for (int i = 0; i < KEMPER_STOMP_COUNT; i++) {
		if (assignment & 1) {
			if (state.state != REMOTE_STATE_STOMP_PARAMETER)
				kemper->toggleStomp(i);
			else {
				if (kemper->parameter.stompIdx != i) {
					kemper->loadPartialParam(i);
					break;
				}
			}
		}
		assignment = assignment >> 1;
	}
}

void KemperRemote::onRigDown(int switchIdx) {
	if (state.state == REMOTE_STATE_STOMP_PARAMETER) {
		state.state = REMOTE_STATE_STOMP_PARAMETER_POST_LOAD;
		for (int i = 0; i < KEMPER_STOMP_COUNT; i++) {
			StompInfo *info = &kemper->state.stomps[i].info;
			if (info->type > 0)
			{
				PGM_KemperParam** params = (PGM_KemperParam**)pgm_read_word_near(&AllStomps[info->PGM_index].params);
				for (int j = 0; j < info->paramCount; j++) {
					PGM_KemperParam *param = (PGM_KemperParam*)pgm_read_word_near(&params[j]);
					newStompParameters[i][j][0] = pgm_read_word_near(&param->number);
				}
			}
		}
		return;
	}
	bool isChanged = false;
	if (state.state!=REMOTE_STATE_LOOPER)
	{
		if (kemper->state.mode == MODE_BROWSE) {
			isChanged = kemper->state.currentRig != rigMap[state.currentPage * SWITCH_RIG_COUNT + switchIdx];
			kemper->setRig(rigMap[state.currentPage * SWITCH_RIG_COUNT + switchIdx]);
		}
		else {
			isChanged = kemper->state.slot != switchIdx;
			kemper->setPerformance(kemper->state.performance, switchIdx);
		}
	}
	if (isChanged && state.state == REMOTE_STATE_STOMP_PARAMETER)
		state.state = REMOTE_STATE_NORMAL;
}

void KemperRemote::onRigUp(int switchIdx) {

}


void KemperRemote::onStompUp(int switchIdx) {
}

void KemperRemote::assignStomps(byte switchId, byte assign) {
	currentStompAssignment[switchId] = assign;
	if (kemper->state.mode == MODE_BROWSE) {
		for (int i=0;i<SWITCH_STOMP_COUNT;i++) {
			stompAssignment[currentRig%SWITCH_STOMP_COUNT][i] = currentStompAssignment[i]; // assign default assignments permanently
			state.isSaved = false;
		}
	} else if (kemper->state.mode == MODE_PERFORM) {
		for (int i=0;i<SWITCH_STOMP_COUNT;i++) {
			stompAssignmentPerform[state.currentSlot][i] = currentStompAssignment[i]; // assign default assignments permanently
			state.isSaved = false;
		}
	}
}

void KemperRemote::assignRig(byte switchId, byte rig) {
	rigMap[state.currentPage * SWITCH_RIG_COUNT + switchId] = rig;
	state.isSaved = false;
}

void KemperRemote::checkSwitchDownLong() {
	if (currentSwitch >= 0 && millis() - switchDownStart>1000) {
		// one switch is pressed more than one second
		// for stomp switches activate stomp assign mode
		if (state.state == REMOTE_STATE_NORMAL) {
			if (currentSwitch >= SWITCH_STOMP_START && currentSwitch < SWITCH_STOMP_START + SWITCH_STOMP_COUNT) {
				state.state = REMOTE_STATE_STOMP_ASSIGN;
				
				for (int i=0;i<KEMPER_STOMP_COUNT;i++) {
					initialStompStates[i] = kemper->state.stomps[i].active;
					changedStomps[i] = false;
				}
				debug(F("Stomp Assign mode activated"));
			}
			if (kemper->state.mode == MODE_BROWSE && currentSwitch >= SWITCH_RIG_START && currentSwitch < SWITCH_RIG_START + SWITCH_RIG_COUNT) {
				state.state = REMOTE_STATE_RIG_ASSIGN;
				debug(F("Rig assign mode activated"));
				rigAssignSwitch = currentSwitch;
				rigAssignRig = kemper->state.currentRig;
			}
		}
	}
	if (currentSwitch >= 0 && millis() - switchDownStart>700) {
		if (currentSwitch == SWITCH_TAP) {
			state.state = REMOTE_STATE_TEMPO_DETECTION;
		}
	}
}

void KemperRemote::checkUpDownScroll() {
	static unsigned long lastUpDownScroll = 0;
	if (millis() - switchDownStart > 1000 && switchStates[SWITCH_UP] && switchStates[SWITCH_DOWN] && !state.isSaved) {
		// save
		save();
	}
	if (state.state == REMOTE_STATE_RIG_ASSIGN && millis() - switchDownStart > 500 && millis() - lastUpDownScroll>150) {
		if (currentSwitch == SWITCH_DOWN && !switchStates[SWITCH_UP]) {
			if (rigAssignRig<RIG_COUNT-2)
				kemper->setRig(++rigAssignRig);
		}
		if (currentSwitch == SWITCH_UP && !switchStates[SWITCH_DOWN]) {
			if (rigAssignRig>0)
				kemper->setRig(--rigAssignRig);		
		}
		lastUpDownScroll = millis();
	}
	if (kemper->state.mode == MODE_PERFORM && millis() - switchDownStart > 500 && millis() - lastUpDownScroll>300) {
		if (currentSwitch == SWITCH_DOWN &&  !switchStates[SWITCH_UP]) {
			if (kemper->state.performance<125)
				kemper->setPerformance(kemper->state.performance+1);
		}
		if (currentSwitch == SWITCH_UP &&  !switchStates[SWITCH_DOWN]) {
			if (kemper->state.performance>0)
				kemper->setPerformance(kemper->state.performance-1);
		}
		lastUpDownScroll = millis();
	}
}

void KemperRemote::checkStompChanges() {
	if (state.state == REMOTE_STATE_STOMP_ASSIGN) {
		for (int i=0;i<KEMPER_STOMP_COUNT;i++) {
			if (initialStompStates[i] != kemper->state.stomps[i].active && !changedStomps[i]) {
				debug(F("STOMPS CHANGED"));
				debug(i);
				changedStomps[i] = true;
			}
		}
	}
}

void KemperRemote::onSwitchDown(int sw) {
	if (state.state == REMOTE_STATE_EXPRESSION_CALIBRATE)
		return;

	if (sw == SWITCH_LOOPER) {
		if (state.state == REMOTE_STATE_LOOPER)
			state.state = REMOTE_STATE_NORMAL;
		else if (state.state == REMOTE_STATE_NORMAL)
			state.state = REMOTE_STATE_LOOPER;
	}

	if (millis() - switchDownStart<500 && sw == SWITCH_LOOPER) { 
		if (expPedals[0].isCalibrated())
		{
			// looper double click - change expression pedal mode
			int modeCount = sizeof(ExpressionPedalModes)/sizeof(ExpressionPedalModes[0]);
			for (int i=0;i<modeCount;i++) {
				if (ExpressionPedalModes[i] == expPedals[0].mode) {
					expPedals[0].mode = ExpressionPedalModes[(i+1)%modeCount];
					switchDownStart = 0;
					return;
				}
			}
		}
	}

	if (millis() - switchDownStart < 500 && sw >= SWITCH_RIG_START && sw < SWITCH_RIG_START+ SWITCH_RIG_COUNT) {

		if (state.state != REMOTE_STATE_STOMP_PARAMETER && state.state != REMOTE_STATE_STOMP_PARAMETER_LOAD)
		{
			state.state = REMOTE_STATE_STOMP_PARAMETER_LOAD;

			bool found = false;
			for (int j = 0; j < SWITCH_STOMP_COUNT && !found; j++) {
				byte assignment = currentStompAssignment[j];

				for (int i = 0; i < KEMPER_STOMP_COUNT; i++) {
					if (assignment & 1) {
						kemper->loadPartialParam(i);
						found = true;
						break;
					}
					assignment = assignment >> 1;
				}
			}
			memset(oldStompParameters, -1, sizeof(oldStompParameters));
			memset(newStompParameters, -1, sizeof(newStompParameters));
			for (int i = 0; i < KEMPER_STOMP_COUNT;i++) {
				StompInfo *info = &kemper->state.stomps[i].info;
				if (info->type>0)
				{
					PGM_KemperParam** params = (PGM_KemperParam**)pgm_read_word_near(&AllStomps[info->PGM_index].params);
					for (int j = 0; j < info->paramCount; j++) {
						PGM_KemperParam *param = (PGM_KemperParam*)pgm_read_word_near(&params[j]);
						oldStompParameters[i][j][0] = pgm_read_word_near(&param->number);
					}
				}
			}
		}
		else {
			state.state = REMOTE_STATE_NORMAL;
		}
	}

	switchDownStart = millis();
	switchStates[sw] = true;
	currentSwitch = sw;

	if (state.state == REMOTE_STATE_LOOPER)
	{
		if (sw == SWITCH_UP)
			kemper->looperUndoDown();
		else if (sw == SWITCH_RIG_START)
			kemper->looperRecordPlayDown();
		else if (sw == SWITCH_RIG_START+1)
			kemper->looperStopEraseDown();
		else if (sw == SWITCH_RIG_START+2)
			kemper->looperTriggerDown();
		else if (sw == SWITCH_RIG_START+3)
			kemper->looperReverseDown();
		else if (sw == SWITCH_RIG_START+4)
			kemper->looperHalfTimeDown();
	}


	if (sw == SWITCH_UP) {
		if (state.state == REMOTE_STATE_STOMP_PARAMETER) {
			kemper->movePartialParam(state.parameterState!=REMOTE_PARAMETER_STATE_VALUE?-1:0, state.parameterState == REMOTE_PARAMETER_STATE_VALUE ?-1:0);
		} else if (state.state == REMOTE_STATE_RIG_ASSIGN) {
			if (rigAssignRig>0)
				kemper->setRig(--rigAssignRig);
		} 
	}
	if (sw == SWITCH_DOWN) {
		if (state.state == REMOTE_STATE_STOMP_PARAMETER) {
			kemper->movePartialParam(state.parameterState!=REMOTE_PARAMETER_STATE_VALUE?1:0, state.parameterState == REMOTE_PARAMETER_STATE_VALUE ?1:0);
		} else if (state.state == REMOTE_STATE_RIG_ASSIGN) {
			if (rigAssignRig<RIG_COUNT-2)
				kemper->setRig(++rigAssignRig);
		}
	}
	if (sw == SWITCH_TUNER) {
		if (kemper->state.mode != MODE_TUNER) {
			kemper->tunerOn();
			debug(F("Activate tuner"));
		} else {
			kemper->tunerOff();
			debug(F("Back to browse"));
		}
	}
	if (sw >= SWITCH_RIG_START && sw < SWITCH_RIG_START + SWITCH_RIG_COUNT) {
		if (state.state == REMOTE_STATE_RIG_ASSIGN) {
			if (sw == rigAssignSwitch) {
				assignRig(rigAssignSwitch-SWITCH_RIG_START, rigAssignRig);
			}
			state.state = REMOTE_STATE_NORMAL;
			kemper->setRig(rigMap[state.currentPage * SWITCH_RIG_COUNT + rigAssignSwitch - SWITCH_RIG_START]);
		}
		else
			onRigDown(sw - SWITCH_RIG_START);
	}
	if (sw >= SWITCH_STOMP_START && sw < SWITCH_STOMP_START + SWITCH_STOMP_COUNT) {
		onStompDown(sw - SWITCH_STOMP_START);
	}
	if (sw == SWITCH_TAP) {
		if (!kemper->parameter.isActive)
			kemper->tapOn();
		else 
			state.parameterState = (state.parameterState+1)%3;
	}

}

void KemperRemote::onSwitchUp(int sw) {
	if (state.state == REMOTE_STATE_EXPRESSION_CALIBRATE) {
		state.state = state.previousState;
		return;
	}

	if (state.state == REMOTE_STATE_LOOPER)
	{
		if (sw == SWITCH_UP)
			kemper->looperUndoUp();
		else if (sw == SWITCH_RIG_START)
			kemper->looperRecordPlayUp();
		else if (sw == SWITCH_RIG_START+1)
			kemper->looperStopEraseUp();
		else if (sw == SWITCH_RIG_START+2)
			kemper->looperTriggerUp();
		else if (sw == SWITCH_RIG_START+3)
			kemper->looperReverseUp();
		else if (sw == SWITCH_RIG_START+4)
			kemper->looperHalfTimeUp();
	}

	if (sw >= SWITCH_RIG_START && sw < SWITCH_RIG_START + SWITCH_RIG_COUNT) {
		onRigUp(sw - SWITCH_RIG_START);
	}
	if (sw >= SWITCH_STOMP_START && sw < SWITCH_STOMP_START + SWITCH_STOMP_COUNT) {
		onStompUp(sw - SWITCH_STOMP_START);
	}	
	if (sw == SWITCH_TAP)
		if (!kemper->parameter.isActive)
			kemper->tapOff();
	if (state.state == REMOTE_STATE_STOMP_ASSIGN) {
		int changedCount = 0;
		byte currentAssignment = currentStompAssignment[currentSwitch - SWITCH_STOMP_START];
		for (int i=0;i<KEMPER_STOMP_COUNT;i++) {
			if (changedStomps[i]) {
				if (changedCount==0)
					currentAssignment = 0;
				currentAssignment = currentAssignment | (1<<i);
				changedCount++;
			}
		}
		debug(F("Stomp assignment result"));
		debug(currentSwitch);
		debug(changedCount);
		debug(currentAssignment);
		debug(F("Current Assignment"));
		for (int i=0;i<SWITCH_STOMP_COUNT;i++) {
			debug(currentStompAssignment[i]);
		}
		assignStomps(currentSwitch - SWITCH_STOMP_START, currentAssignment);
		debug(F("Modified Assignment"));
		for (int i=0;i<SWITCH_STOMP_COUNT;i++) {
			debug(currentStompAssignment[i]);
		}
		state.state = REMOTE_STATE_NORMAL;
		debug(F("Normal Mode"));
	}

	if (sw == SWITCH_UP) {
		if (state.state != REMOTE_STATE_STOMP_PARAMETER && state.state != REMOTE_STATE_RIG_ASSIGN && saveUpDown == 0) {
			if (kemper->state.mode == MODE_PERFORM) {
				kemper->setPerformance(kemper->state.performance - 1);
			}
			else if (kemper->state.mode == MODE_BROWSE) {
				state.currentPage = (byte)max(0, state.currentPage - 1);
			}
			debug(F("Page changed"));
			debug(state.currentPage);
		}
		saveUpDown = max(saveUpDown - 1, 0);
	}
	if (sw == SWITCH_DOWN) {
		if (state.state != REMOTE_STATE_STOMP_PARAMETER && state.state != REMOTE_STATE_RIG_ASSIGN && saveUpDown == 0) {
			if (kemper->state.mode == MODE_PERFORM) {
				kemper->setPerformance(kemper->state.performance + 1);
			}
			else if (kemper->state.mode == MODE_BROWSE) {
				state.currentPage = (byte)min(kemper->state.mode == MODE_PERFORM ? 125 : 25, state.currentPage + 1);
			}
		}
		saveUpDown = max(saveUpDown - 1, 0);
	}

	if (state.state == REMOTE_STATE_TEMPO_DETECTION)
		state.state = REMOTE_STATE_NORMAL;

	currentSwitch = -1;
	switchStates[sw] = false;
}

void KemperRemote::updateLeds() {
		
	int l = 0;
	for (byte i = 0;i<SWITCH_RIG_COUNT; i++) {
		bool rigActive = false;
		
		if (kemper->state.mode == MODE_PERFORM) {
			rigActive = i == kemper->state.slot;
		} else if (state.state == REMOTE_STATE_RIG_ASSIGN) {
			rigActive = i == rigAssignSwitch - SWITCH_RIG_START;
			if (!kemper->state.tempoLed) // blink //millis()%1000<500
				rigActive = false;
		} else {
			 rigActive = kemper->state.currentRig == rigMap[state.currentPage * SWITCH_RIG_COUNT + i];
		}
		if (state.state == REMOTE_STATE_STOMP_PARAMETER && !kemper->state.tempoLed) //millis() % 500 < 250
			rigActive = false;
		leds[l++] = rigActive?0x7f:0;
		leds[l++] = rigActive?0x7f:0;
		leds[l++] = rigActive?0x7f:0;
	} 

	for (byte i = 0;i<SWITCH_STOMP_COUNT; i++) {
		byte assignment = currentStompAssignment[i];
		int k = 0;
		for (byte j=0;j<KEMPER_STOMP_COUNT && k<2;j++) {
			if ((assignment & 1 && kemper->state.stomps[j].info.type>0)) { //
				bool isOff = state.state == REMOTE_STATE_STOMP_PARAMETER && kemper->parameter.stompIdx == j && !kemper->state.tempoLed; // millis() % 500 < 250;
				leds[l++] = isOff ? 0 : (kemper->state.stomps[j].info.color.r >> 1);
				leds[l++] = isOff ? 0 : (kemper->state.stomps[j].info.color.g >> 1);
				leds[l++] = isOff ? 0 : (kemper->state.stomps[j].info.color.b >> 1);
				leds[l++] = kemper->state.stomps[j].active?0x7f:0;
				leds[l++] = kemper->state.stomps[j].active?0x7f:0;
				leds[l++] = kemper->state.stomps[j].active?0x7f:0;
				k++;
			}
			assignment = assignment >> 1;
		}
		for (byte j=k*6;j<2*6;j++) {
			leds[l++] = 0;
		}
	}

	leds[l++] = kemper->state.tempoLed && kemper->state.tempoEnabled?0x7f:0;
	leds[l++] = kemper->state.tempoLed && kemper->state.tempoEnabled?0x7f:0;
	leds[l++] = kemper->state.tempoLed && kemper->state.tempoEnabled?0x7f:0;

	if (kemper->state.tune>0 && kemper->state.mode == MODE_TUNER) {
		float mid = 8191; // 0x1fff;
		float range = 1600;
		float width = 1400;
		float v1 = ((mid - range/2) - kemper->state.tune) / (width/2);
		float v2 = abs((int)(kemper->state.tune - mid)) / (range);
		float v3 = (kemper->state.tune - (mid + range/2)) / (width/2);
		v1 = min(max(v1, 0), 1);
		v2 = min(max(2-v2, 0), 1);
		v3 = min(max(v3, 0), 1);
		leds[l++] = (byte)(0x00 * v1);
		leds[l++] = (byte)(0x7f * v1);
		leds[l++] = (byte)(0x00 * v1);
		leds[l++] = (byte)(0x7f * v2);
		leds[l++] = (byte)(0x7f * v2);
		leds[l++] = (byte)(0x7f * v2);
		leds[l++] = (byte)(0x00 * v3);
		leds[l++] = (byte)(0x7f * v3);
		leds[l++] = (byte)(0x00 * v3);
	}
	else {
		for (int i=0;i<9;i++)
			leds[l++] = 0x0;
	}
	leds[l++] = state.state == REMOTE_STATE_LOOPER?0x7f:0;
	leds[l++] = state.state == REMOTE_STATE_LOOPER?0x7f:0;
	leds[l++] = state.state == REMOTE_STATE_LOOPER?0x7f:0;

	for (int i=0;i<sizeof(ExpressionPedalModes)/sizeof(ExpressionPedalModes[0]);i++)
	{
		leds[l++] = expPedals[0].mode == ExpressionPedalModes[i] && expPedals[0].isCalibrated()?0x7f:0;
		leds[l++] = expPedals[0].mode == ExpressionPedalModes[i] && expPedals[0].isCalibrated()?0x7f:0;
		leds[l++] = expPedals[0].mode == ExpressionPedalModes[i] && expPedals[0].isCalibrated()?0x7f:0;
	}
}

void KemperRemote::save() {
	/* //@ersin - add save here
	EEPROM.put( 0, stompAssignment );
	EEPROM.put( sizeof(stompAssignment), stompAssignmentPerform );
	EEPROM.put( sizeof(stompAssignment)+sizeof(stompAssignmentPerform), rigMap);
	*/
	EEPROM.put((currentRig / SWITCH_RIG_COUNT)*sizeof(stompAssignment), stompAssignment);
	EEPROM.put(BROWSE_PAGE_COUNT*sizeof(stompAssignment) + (kemper->state.performance)*sizeof(stompAssignmentPerform), stompAssignmentPerform);
	EEPROM.put(sizeof(stompAssignment)*BROWSE_PAGE_COUNT + sizeof(stompAssignmentPerform)*RIG_COUNT, rigMap);
	state.isSaved = true;
	saveUpDown = 2;
}