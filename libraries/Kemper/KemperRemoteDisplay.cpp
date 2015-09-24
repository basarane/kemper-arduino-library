
#include "KemperRemoteDisplay.h"

USING_NAMESPACE_KEMPER

KemperRemoteDisplay::KemperRemoteDisplay(AbstractKemper* _kemper, KemperRemote* _kemperRemote, AbstractDisplay* _display) {
	kemper = _kemper;
	kemperRemote = _kemperRemote;
	display = _display;
	lastDisplay = 0;
	lastTempoTime = 0;
	lastTunerTime = 0;
	lastKemperRemoteState.isSaved = true;
	firstRun = 0;
	writeToLayer = 0;
}

void KemperRemoteDisplay::welcome() {
	int width = display->width;
	int height = display->height;
	display->fillRect(0, 0, width - 1, height - 1, getColor(0, 0, 255));
	char txt[] = "Connecting to KEMPER...";
	display->drawText(0, 0, width - 1, height - 1, TextAlignCenter, TextAlignMiddle, 40, txt, strlen(txt), getColor(255, 255, 255));
}

void KemperRemoteDisplay::draw() {

	if (millis() - lastDisplay>50) {
		// make sure all info shown after 1 sec of first boot

		lastDisplay = millis();

		int width = display->width;
		int height = display->height;

		if (!kemper->state.senseReceived)
		{
			if (lastKemperState.senseReceived) {
				welcome();
			}
		}
		else
		{
			bool forceShow = false;
			if (firstRun == 0 && millis() > 1000) {
				firstRun = 1;
				forceShow = true;
			}
			if (!lastKemperState.senseReceived)
				forceShow = true;

			if (kemper->state.mode != MODE_TUNER && kemperRemote->state.state!=REMOTE_STATE_EXPRESSION_CALIBRATE)
			{
				static unsigned long saveTime = 0;
				static bool lastShowSave = false;

				bool isSaved = !lastKemperRemoteState.isSaved && kemperRemote->state.isSaved;
				if (isSaved) {
					saveTime = millis();
				}
				bool showSave = millis() - saveTime < 2000 && saveTime>0;
				bool updateStomps = (lastKemperRemoteState.state == REMOTE_STATE_RIG_ASSIGN && kemperRemote->state.state != REMOTE_STATE_RIG_ASSIGN)
						|| (lastKemperRemoteState.state == REMOTE_STATE_EXPRESSION_CALIBRATE && kemperRemote->state.state != REMOTE_STATE_EXPRESSION_CALIBRATE)
						|| (
							(lastKemperRemoteState.state == REMOTE_STATE_STOMP_PARAMETER || lastKemperRemoteState.state == REMOTE_STATE_STOMP_PARAMETER_POST_LOAD)
							&& (lastKemperRemoteState.state != REMOTE_STATE_STOMP_PARAMETER && lastKemperRemoteState.state != REMOTE_STATE_STOMP_PARAMETER_POST_LOAD)
							)
						|| lastKemperRemoteState.currentParameters != kemperRemote->state.currentParameters
						|| lastKemperRemoteState.currentParametersChanged != kemperRemote->state.currentParametersChanged
						|| (lastKemperState.mode == MODE_TUNER && kemper->state.mode != MODE_TUNER) || lastShowSave != showSave
						|| (lastKemperState.mode != MODE_BROWSE && kemper->state.mode == MODE_BROWSE)
						|| (lastKemperState.mode != MODE_PERFORM && kemper->state.mode == MODE_PERFORM)
						|| forceShow;
				if (isSaved) {
					display->fillRect(0, 0, width-1, height/2, getColor(0,0,255));
					char* saveText = "SETTINGS SAVED";
					display->drawText(0, 0, width-1, height/2, TextAlignCenter, TextAlignMiddle, 40, saveText, strlen(saveText), getColor(255,255,255));
				}

				if (!showSave) {
					if (updateStomps) {
						display->fillRect(0, 0, width-1, height/2, getColor(255,255,255));
					}
					for (int i=0;i<KEMPER_STOMP_COUNT;i++) {
						if (
							(
							(lastKemperState.stomps[i].info.type!=kemper->state.stomps[i].info.type || lastKemperState.stomps[i].active!=kemper->state.stomps[i].active) 
								&& kemperRemote->state.state != REMOTE_STATE_RIG_ASSIGN && kemper->state.mode != MODE_TUNER
							)
							|| updateStomps)
						{
							// new horizontal layout
							int sw = width / 2;
							int sh = height / 8; // x/2*2 = x
							int x = (i/4)*sw;
							int y = (i%4)*sh;

							StompState stomp = kemper->state.stomps[i];
							if (stomp.info.type>0) {
								int color = getColor(0, 0, 0);
								Color sColor = stomp.info.color;
								if (stomp.info.color.b == 255 && stomp.info.color.r == 0 && stomp.info.color.g == 0 || stomp.info.color.b == 0 && stomp.info.color.r == 255 && stomp.info.color.g == 0) {
									color = getColor(255,255,255);
								}
								int alpha = 80;
								if (!stomp.active) {
									sColor.r = (255*alpha + (100-alpha)*sColor.r) / 100;
									sColor.g = (255*alpha + (100-alpha)*sColor.g) / 100;
									sColor.b = (255*alpha + (100-alpha)*sColor.b) / 100;
									color = getColor(180,180,180);
								}

								// new horizontal text layout
								display->fillRoundRect(x+1, y, sw-2, sh-2, 8, getColor(sColor.r, sColor.g, sColor.b));
								display->drawText(x+1, y, sw-2, sh-2, TextAlignCenter, TextAlignMiddle, 24, stomp.info.name, strlen(stomp.info.name), color);

								if (kemperRemote->state.currentParameters) {
									byte *p = kemperRemote->state.currentParameters + 2;
									byte count = *p;
									p++;
									bool markIt = false;
									for (int j = 0; j < count; j++) {
										if (*(p + j * 6) == i)
											markIt = true;
									}
									if (markIt)
										display->fillRoundRect(x + sw- (sh - 15) / 2 - 15, y + (sh-15)/2, 15, 15, 4, getColor(0, 0, 0));
								}

							} else {
								display->fillRoundRect(x+1, y, sw-2, sh-2, 8, getColor(220, 220, 220));
							}
						}
					}
				}
				lastShowSave = showSave;
			}

			bool updateCurrentPage = kemperRemote->state.currentPage != lastKemperRemoteState.currentPage || forceShow;
			bool parameterChanged =
				(kemperRemote->state.state == REMOTE_STATE_STOMP_PARAMETER && lastKemperRemoteState.state != REMOTE_STATE_STOMP_PARAMETER);
			bool bottomChanged = (kemperRemote->state.state != REMOTE_STATE_STOMP_ASSIGN && lastKemperRemoteState.state == REMOTE_STATE_STOMP_ASSIGN)
						|| (kemperRemote->state.state != REMOTE_STATE_TEMPO_DETECTION && lastKemperRemoteState.state == REMOTE_STATE_TEMPO_DETECTION)
						|| (kemper->state.mode == MODE_PERFORM && lastKemperState.mode != MODE_PERFORM)
						|| (kemper->state.mode == MODE_BROWSE && lastKemperState.mode != MODE_BROWSE)
						|| (kemperRemote->state.state != REMOTE_STATE_STOMP_PARAMETER && lastKemperRemoteState.state == REMOTE_STATE_STOMP_PARAMETER)
						|| parameterChanged
						|| forceShow;
			parameterChanged = parameterChanged || (kemperRemote->state.state == REMOTE_STATE_STOMP_PARAMETER && (kemper->state.parameterState!=lastKemperState.parameterState || kemperRemote->state.parameterState!=lastKemperRemoteState.parameterState));

			if (bottomChanged) {
				display->fillRect(0, height/2, width-1, height/2-1, getColor(255,255,255));
			}
			if (kemperRemote->state.state!=REMOTE_STATE_STOMP_PARAMETER && display->supportsLayers)
				display->layerEffect(1);

			if (kemperRemote->state.state == REMOTE_STATE_STOMP_ASSIGN && lastKemperRemoteState.state != REMOTE_STATE_STOMP_ASSIGN) {
				display->fillRect(0, height/2, width-1, height/2-1, getColor(255,0,0));
				char* stompAssignText = "STOMP ASSIGNMENT";
				display->drawText(0, height/2, width-1, height/2-1, TextAlignCenter, TextAlignMiddle, 48, stompAssignText, strlen(stompAssignText), getColor(255,255,255));
			} else if (parameterChanged) {
				int ph = 20;
				for (int i=0;i<5;i++) {
					bool selected = kemper->parameter.currentParam - kemper->parametopopoppooppopopopopopopopopoopopopoppoer.startParamIndex == i;
				
					display->fillRoundRect(10, height/2 + 10 + ph*i, 220, ph, 5, selected?(kemperRemote->state.parameterState == REMOTE_PARAMETER_STATE_VALUE?getColor(180,180,255): getColor(0, 0, 255)):getColor(255,255,255));
					if (i<kemper->parameter.paramCount)
						display->drawText(10, height/2 + 10 + ph*i, 220, ph, TextAlignCenter, TextAlignMiddle, 16, kemper->parameter.params[i].name, strlen(kemper->parameter.params[i].name), !selected || kemperRemote->state.parameterState == REMOTE_PARAMETER_STATE_VALUE?0:getColor(255,255,255));
				}


				if (kemper->parameter.totalOptionCount>0) {
					if (display->supportsLayers)
					{
						display->layerEffect(1);
					}
					display->fillRect(width/2+1,height/2+10, width/2-5, height/2-33, getColor(255,255,255));

					for (int i=0;i<NUMBER_OF_OPTIONS_IN_LIST;i++) {
						bool selected = kemper->parameter.currentOption - kemper->parameter.startOptionIndex == i;
						display->fillRoundRect(250, height/2 + 10 + ph*i, 220, ph, 5, selected? (kemperRemote->state.parameterState == REMOTE_PARAMETER_STATE_PARAMETER ? getColor(180, 180, 255) : getColor(0, 0, 255)) :getColor(255,255,255));
						if (i<kemper->parameter.optionCount)
							display->drawText(260, height/2 + 10 + ph*i, 220, ph, TextAlignCenter, TextAlignMiddle, 16, kemper->parameter.options[i].name, strlen(kemper->parameter.options[i].name), !selected||kemperRemote->state.parameterState == REMOTE_PARAMETER_STATE_PARAMETER ? 0 : getColor(255, 255, 255));
					}
				} else {
					int x0 = 250;
					int y0 = 240;
					int h = 50;
					int w = 220;
					if (display->supportsLayers)
					{
						display->writeTo(writeToLayer);
					}
					display->fillRect(x0,height/2+10, w, height/2-33, getColor(255,255,255));
				
					long maxVal = ((1<<14)-1);
					if (kemper->parameter.params[kemper->parameter.currentParam - kemper->parameter.startParamIndex].number == 20)
						maxVal = 127;
						//maxVal = kemper->parameter.valueType.maxParam;
					float val = (float)kemper->parameter.currentValue / maxVal;
					val = max(0, val);
					val = min(1, val);
					int wdist = max(1, w*val);

					int pcolor = kemperRemote->state.parameterState == REMOTE_PARAMETER_STATE_PARAMETER ? 0 : getColor(0, 0, 255);

					if (kemper->parameter.valueType.minValue >= 0)
					{
						int hdist = max(1, h*val);
						display->fillTriangle(x0, y0, x0+wdist, y0, (long)x0+wdist, y0-hdist, pcolor);
					}
					else {
						int hdist = max(1, abs((int)(h*2*(val-0.5))));
						display->fillTriangle(x0+w/2, y0, x0 + wdist, y0, (long)x0 + wdist, y0 - hdist, pcolor);
					}
					char txt[20];
					KemperParamValue *valueType = &kemper->parameter.valueType;
					float displayVal = 0;
					if (valueType->exponential) {
						displayVal = exp(val*log(valueType->maxValue) + (1 - val)*log(valueType->minValue));
					}
					else {
						displayVal = val*(valueType->maxValue - valueType->minValue) + valueType->minValue;
					}
					char valStr[10];
					ftoa(valStr, displayVal, 1);

					sprintf(txt, "%s%s %s", valueType->minValue<0&&val>0.5?"+":"", valStr, valueType->suffix);
					display->drawText(x0, y0 - h-20, w, 40, TextAlignCenter, TextAlignTop, 30, txt, strlen(txt), pcolor);
					if (display->supportsLayers)
					{
						display->layerEffect(writeToLayer==0?2:1);
						display->writeTo(1);
						writeToLayer = (writeToLayer+1)%2;
					}

				}

			} else if (kemper->state.mode == MODE_BROWSE) {
				if (bottomChanged || strcmp(lastKemperState.rigName, kemper->state.rigName)) {
					display->fillRect(0, height/2, width-1, height/2-21, getColor(255,255,255));
					display->drawText(0, height/2 + 20, width-1, 52, TextAlignCenter, TextAlignMiddle, 34, kemper->state.rigName, strlen(kemper->state.rigName), 0);
					updateCurrentPage = true;
				}
			} else if (kemper->state.mode == MODE_PERFORM) {
					if (bottomChanged || strcmp(lastKemperState.performanceNames[0], kemper->state.performanceNames[0])!=0) 
					{
						display->fillRect(10, height/2 + 10, width-1, 72, getColor(255,255,255));
						display->drawText(10, height/2 + 10, width-1, 72, TextAlignCenter, TextAlignMiddle, 34, kemper->state.performanceNames[0], strlen(kemper->state.performanceNames[0]), 0);
					}
					for (int i = 0;i<5;i++)
					{
						if (bottomChanged || strcmp(lastKemperState.performanceNames[i+1], kemper->state.performanceNames[i+1])!=0 || (i==kemper->state.slot && i!=lastKemperState.slot) || (i!=kemper->state.slot && i==lastKemperState.slot)) 
						{
							uint16_t color = 0;
							uint16_t bgColor = getColor(220,220,220);
							if (i==kemper->state.slot) {
								color = getColor(255,255,255);
								bgColor = 0;
							}
							display->fillRect(i*width/5, height/2 + 80, width/5-2, 35, bgColor);
							display->drawText(i*width/5, height/2 + 80, width/5-2, 35, TextAlignCenter, TextAlignMiddle, 12, kemper->state.performanceNames[i+1], strlen(kemper->state.performanceNames[i+1]), color);
						}
					}
			}

			if (updateCurrentPage && kemper->state.mode == MODE_BROWSE)
			{
				char pageNo[20];
				display->fillRect(width-168, height-42, 167, 20, getColor(255,0,0));
				sprintf(pageNo, "Page: %d", kemperRemote->state.currentPage+1);
				display->drawText(width-168, height - 42, 167, 21, TextAlignCenter, TextAlignMiddle, 16, pageNo, strlen(pageNo), getColor(255,255,255));
			}
			if (kemperRemote->state.state == REMOTE_STATE_RIG_ASSIGN && lastKemperRemoteState.state != REMOTE_STATE_RIG_ASSIGN) {
				display->fillRect(0, 0, width-1, height/2, getColor(255,0,0));
				char* rigAssignText = "RIG ASSIGN MODE";
				display->drawText(0, 0, width-1, 40, TextAlignCenter, TextAlignMiddle, 40, rigAssignText, strlen(rigAssignText), getColor(255,255,255));
				char* rigAssignText2 = "Use up/down switches to change the preset. Press blinking switch to apply";
				display->drawText(0, 50, width-2, height/2-50, TextAlignCenter, TextAlignMiddle, 26, rigAssignText2, strlen(rigAssignText2), getColor(255,255,255));
			} 
			if (kemperRemote->state.state == REMOTE_STATE_TEMPO_DETECTION) {
				char* tempoText = "BEAT DETECTION";
				if (lastKemperRemoteState.state != REMOTE_STATE_TEMPO_DETECTION)
				{
					display->fillRect(0, height/2, width-1, height/2-1, getColor(255,255,255));
					display->fillRoundRect(50, height/2 + 20, width-100, height/2-40, 10, getColor(248,140,176));
					display->drawText(0, height/2 + 5, width-1, height/3, TextAlignCenter, TextAlignMiddle, 36, tempoText, strlen(tempoText), 0);
				}
				char tempo[20];
				ftoa(tempo, 1.0*kemper->state.tempo/64, 1);
			
				if (millis() - lastTempoTime > 50)
				{
					display->fillRect(100, height/2 + 70, width-200, 45, getColor(248,140,176));
					display->drawText(0, height/2 + 65, width-1, 45, TextAlignCenter, TextAlignMiddle, 36, tempo, strlen(tempo), 0);
					lastTempoTime = millis();
				}
			}

			bool expChanged = kemperRemote->state.expPedalState != lastKemperRemoteState.expPedalState;

			if (bottomChanged || (kemperRemote->state.state != REMOTE_STATE_STOMP_ASSIGN && (expChanged || lastKemperRemoteState.state == REMOTE_STATE_STOMP_ASSIGN))) {
				int expWidth = width / EXPRESSION_PEDAL_COUNT;

				display->fillRect(0, height - 21, width - 1, 20, getColor(0, 0, 255));
				for (int i = 0; i < EXPRESSION_PEDAL_COUNT; i++)
				{
					char txt[20];
					if (kemperRemote->expPedals[i].isCalibrated()) {
						sprintf(txt, "Exp %d: %s", (i + 1), EXPRESSION_MODE_STR(kemperRemote->expPedals[i].mode));
						display->drawText(i*expWidth, height - 21, expWidth, 20, TextAlignLeft, TextAlignMiddle, 12, txt, strlen(txt), getColor(255, 255, 255));
					}
					else {
						sprintf(txt, "Exp %d: Calib.", (i + 1));
						display->drawText(i*expWidth, height - 21, expWidth, 20, TextAlignLeft, TextAlignMiddle, 12, txt, strlen(txt), getColor(255, 255, 255));
					}
				}
			}
			if (kemperRemote->state.state == REMOTE_STATE_EXPRESSION_CALIBRATE) {
				if (lastKemperRemoteState.state != REMOTE_STATE_EXPRESSION_CALIBRATE) {
					display->fillRect(0, 0, width-1, height/2, getColor(255,0,0));
					char* calibrateText = "CALIBRATE EXPR. PEDAL";
					display->drawText(0, 0, width-1, 40, TextAlignCenter, TextAlignMiddle, 40, calibrateText, strlen(calibrateText), getColor(255,255,255));
					char* calibrateText2 = "Move expression pedal to heal and toe positions several times to calibrate. Press any switch to exit";
					display->drawText(0, 60, width-1, height/2-70, TextAlignCenter, TextAlignMiddle, 26, calibrateText2, strlen(calibrateText2), getColor(255,255,255));
				}
			} else if (kemper->state.mode == MODE_TUNER && millis() - lastTunerTime > 50) {
				int lw = 4;
				int w1 = 80;
				int w2 = 60;

				int x = 150 * ((1.0*kemper->state.tune) / ((1<<14)-1) - 0.5)*2 + 240;
				int w = width;
				int h = height/2;

				display->fillRect(0, 0, w-1, h-1, getColor(255,255,255));

				display->fillRect(w/2-w1/2, 0, w1, h-1, getColor(220,220,220));
				if (kemper->state.tune>0)
					display->fillRoundRect(x-w2/2, h*0.1, w2, h-h*0.2, w2/2-5, getColor(255,0,0));
				display->fillRect(w/2-lw/2, 0, lw, h-1, getColor(0,0,0));
				display->fillRect(0, h/2-lw/2, w-1, lw, getColor(0,0,0));

				static char keys[12][3] = {"A ", "A#", "B ", "C ", "C#", "D ", "D#", "E ", "F ", "F#", "G ", "G#"};
				int key = (12*100+kemper->state.key-45)%12;
				if (kemper->state.key>0)
					display->drawText(10, 10, 0, 0, TextAlignLeft, TextAlignTop, 40, keys[key], strlen(keys[key]), 0);
				lastTunerTime = millis();
			}
		}
		lastKemperState = kemper->state;
		lastKemperRemoteState = kemperRemote->state;
	}
}