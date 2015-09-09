
#include "KemperRemoteDisplay.h"

USING_NAMESPACE_KEMPER

KemperRemoteDisplay::KemperRemoteDisplay(AbstractKemper* _kemper, KemperRemote* _kemperRemote, AbstractDisplay* _display) {
	kemper = _kemper;
	kemperRemote = _kemperRemote;
	display = _display;
	lastDisplay = 0;
	lastTempoTime = 0;
	lastTunerTime = 0;
}

void KemperRemoteDisplay::draw() {
	if (millis() - lastDisplay>250) {
		lastDisplay = millis();
		//display.fillRect(10, 13, 30, 200, getColor(0, 0, 255));
		//display.fillRect(0, 50, 200, 30, getColor(0, 255, 255));
		//display.fillRect(200, 200, 30, 90, getColor(255, 0, 255));
		//display.drawRect(100, 100, 30, 90, getColor(255, 0, 255));
		int width = display->width;
		int height = display->height;
		if (kemper->state.mode != MODE_TUNER)
		{
			if ((lastKemperRemoteState.state == REMOTE_STATE_RIG_ASSIGN && kemperRemote->state.state != REMOTE_STATE_RIG_ASSIGN ) 
					|| (lastKemperState.mode == MODE_TUNER && kemper->state.mode != MODE_TUNER)) {
				display->fillRect(0, 0, width-1, height/2, getColor(255,255,255));
			}
			for (int i=0;i<KEMPER_STOMP_COUNT;i++) {
				if (
					(
						(lastKemperState.stomps[i].info!=kemper->state.stomps[i].info || lastKemperState.stomps[i].active!=kemper->state.stomps[i].active) 
						&& kemperRemote->state.state != REMOTE_STATE_RIG_ASSIGN && kemper->state.mode != MODE_TUNER
					)
					|| (lastKemperRemoteState.state == REMOTE_STATE_RIG_ASSIGN && kemperRemote->state.state != REMOTE_STATE_RIG_ASSIGN ) 
					|| (lastKemperState.mode == MODE_TUNER && kemper->state.mode != MODE_TUNER))
				{
					static char textHolder[64];
					int sw = width / KEMPER_STOMP_COUNT;
					int sh = height/2;
					int x = i*sw;
					StompState stomp = kemper->state.stomps[i];
					if (stomp.info) {
						int color = getColor(0, 0, 0);
						Color sColor = Color(*(stomp.info->color));
						if (stomp.info->color->b == 255 && stomp.info->color->r == 0 && stomp.info->color->g == 0 || stomp.info->color->b == 0 && stomp.info->color->r == 255 && stomp.info->color->g == 0) {
							color = getColor(255,255,255);
						}
						int alpha = 80;
						if (!stomp.active) {
							sColor.r = (255*alpha + (100-alpha)*sColor.r) / 100;
							sColor.g = (255*alpha + (100-alpha)*sColor.g) / 100;
							sColor.b = (255*alpha + (100-alpha)*sColor.b) / 100;
							color = getColor(180,180,180);
						}

						display->fillRect(x+1, 1, sw-2, sh-2, getColor(sColor.r, sColor.g, sColor.b));
						display->drawText(x+8, sh - 6, 24, stomp.info->name.c_str(), strlen(stomp.info->name.c_str()), color, 27000, 8);
					} else {
						display->fillRect(x+1, 1, sw-2, sh-2, getColor(220, 220, 220));
					}
				}
			}
		}
		bool updateCurrentPage = kemperRemote->state.currentPage != lastKemperRemoteState.currentPage;
		bool bottomChanged = (kemperRemote->state.state != REMOTE_STATE_STOMP_ASSIGN && lastKemperRemoteState.state == REMOTE_STATE_STOMP_ASSIGN)
					|| (kemperRemote->state.state != REMOTE_STATE_TEMPO_DETECTION && lastKemperRemoteState.state == REMOTE_STATE_TEMPO_DETECTION)
					|| (kemper->state.mode == MODE_PERFORM && lastKemperState.mode != MODE_PERFORM);
		if (bottomChanged) {
			display->fillRect(0, height/2, width-1, height/2-1, getColor(255,255,255));
		}
		if (kemperRemote->state.state == REMOTE_STATE_STOMP_ASSIGN && lastKemperRemoteState.state != REMOTE_STATE_STOMP_ASSIGN) {
			display->fillRect(0, height/2, width-1, height/2-1, getColor(255,0,0));
			char* stompAssignText = "STOMP ASSIGN MODE";
			display->drawText(30, height/2 + 20, 48, stompAssignText, strlen(stompAssignText), getColor(255,255,255), 0, 12);
		} else if (kemper->state.mode == MODE_BROWSE) {
			if (bottomChanged || lastKemperState.rigName!=kemper->state.rigName) {
				display->fillRect(0, height/2, width-1, height/2-1, getColor(255,255,255));
				display->drawText(30, height/2 + 20, 48, kemper->state.rigName.c_str(), strlen(kemper->state.rigName.c_str()), 0, 0);
				updateCurrentPage = true;
			}
		} else if (kemper->state.mode == MODE_PERFORM) {
				if (bottomChanged || lastKemperState.performanceNames[0] != kemper->state.performanceNames[0]) 
				{
					display->fillRect(10, height/2 + 20, width-1, 52, getColor(255,255,255));
					display->drawText(10, height/2 + 20, 48, kemper->state.performanceNames[0].c_str(), strlen(kemper->state.performanceNames[0].c_str()), 0, 0);
				}
				for (int i = 0;i<5;i++)
				{
					if (bottomChanged || lastKemperState.performanceNames[i+1] != kemper->state.performanceNames[i+1] || (i==kemper->state.slot && i!=lastKemperState.slot) || (i!=kemper->state.slot && i==lastKemperState.slot)) 
					{
						uint16_t color = 0;
						uint16_t bgColor = getColor(220,220,220);
						if (i==kemper->state.slot) {
							color = getColor(255,255,255);
							bgColor = 0;
						}
						display->fillRect(i*width/5, height/2 + 100, width/5-2, 52, bgColor);
						display->drawText(i*width/5, height/2 + 110, 24, kemper->state.performanceNames[i+1].c_str(), strlen(kemper->state.performanceNames[i+1].c_str()), color, 0);
					}
				}
		}
		if (updateCurrentPage && kemper->state.mode == MODE_BROWSE)
		{
			char pageNo[20];
			display->fillRect(width-168, height-42, 167, 41, getColor(255,0,0));
			sprintf(pageNo, "Page: %d", kemperRemote->state.currentPage+1);
			display->drawText(width-160, height - 34, 30, pageNo, strlen(pageNo), getColor(255,255,255), 0);
		}
		if (kemperRemote->state.state == REMOTE_STATE_RIG_ASSIGN && lastKemperRemoteState.state != REMOTE_STATE_RIG_ASSIGN) {
			display->fillRect(0, 0, width-1, height/2, getColor(255,0,0));
			char* rigAssignText = "RIG ASSIGN MODE";
			display->drawText(5, 5, 40, rigAssignText, strlen(rigAssignText), getColor(255,255,255), 0);
			char* rigAssignText2 = "Use up/down switches to change the preset. Press blinking switch to apply";
			display->drawText(5, 50, 26, rigAssignText2, strlen(rigAssignText2), getColor(255,255,255), 0, 30);
		} 
		if (kemperRemote->state.state == REMOTE_STATE_TEMPO_DETECTION) {
			char* tempoText = "BEAT DETECTION";
			if (lastKemperRemoteState.state != REMOTE_STATE_TEMPO_DETECTION)
			{
				display->fillRect(0, height/2, width-1, height/2-1, getColor(255,255,255));
				display->drawText(30, height/2 + 20, 36, tempoText, strlen(tempoText), 0, 0);
			}
			char tempo[20];
			//debug(kemper->state.tempo);
			//sprintf(tempo, "%d", 1.0*kemper->state.tempo/64);
			//sprintf(tempo, "%d", kemper->state.tempo/64);
			ftoa(tempo, 1.0*kemper->state.tempo/64, 1);
			//itoa(kemper->state.tempo, tempo, 10);
			//debug(tempo);
			
			if (millis() - lastTempoTime > 50)
			{
				display->fillRect(30, height/2 + 80, 150, 45, getColor(255,255,255));
				display->drawText(30, height/2 + 80, 48, tempo, strlen(tempo), 0, 0);
				lastTempoTime = millis();
			}
		}
		if (kemper->state.mode == MODE_TUNER && millis() - lastTunerTime > 50) {
			int lw = 4;
			int w1 = 80;
			int w2 = 60;

			int x = 150 * ((1.0*kemper->state.tune) / ((1<<14)-1) - 0.5)*2 + 240;
			int w = width;
			int h = height/2;

			display->fillRect(0, 0, w, h, getColor(255,255,255));

			display->fillRect(w/2-w1/2, 0, w1, h-1, getColor(220,220,220));
			if (kemper->state.tune>0)
				display->fillRect(x-w2/2, h*0.1, w2, h-h*0.2, getColor(255,0,0));
			display->fillRect(w/2-lw/2, 0, lw, h-1, getColor(0,0,0));
			display->fillRect(0, h/2-lw/2, w-1, lw, getColor(0,0,0));

			static char keys[12][3] = {"A ", "A#", "B ", "C ", "C#", "D ", "D#", "E ", "F ", "F#", "G ", "G#"};
			int key = (12*100+kemper->state.key-45)%12;
			if (kemper->state.key>0)
				display->drawText(10, 10, 40, keys[key], strlen(keys[key]), 0, 0);
			lastTunerTime = millis();
		}

		lastKemperState = kemper->state;
		lastKemperRemoteState = kemperRemote->state;
	}
}