
#include "KemperRemoteDIYWin.h"

#include <windows.h>
#include <stdio.h>
#include "KemperRemoteDisplay.h"

#include "VirtualDisplaySerializer.h"
#include "SerialDisplaySerializer.h"



#include <iostream>
#include <cstdlib>
#include "RtMidi.h"


USING_NAMESPACE_KEMPER

int main(int argc, char**argv) {

	//Sleep(10000);
	if (argc < 2) {
		printf("USAGE: KemperRemoteDIYWin.exe <mode> [<midi_in_port>] [<midi_out_port>]\n\n");
		printf("mode = 0: Kemper Mock\n");
		printf("mode = 1: Kemper MIDI\n");
		exit(1);
	}
	int mode = atoi(argv[1]);

	AbstractKemper* kemper;

	if (mode == 0) {
		KemperMock kemperMock;
		kemper = &kemperMock;
	} else {
		Kemper kemperReal;
		kemperReal.setMidiPorts(0, 1);
		kemperReal.begin();
		kemper = &kemperReal;
	}
	kemper->state.mode = MODE_BROWSE;

	//kemper->setRig(2);

	KemperRemote kemperRemote(kemper);

	VirtualDisplaySerializer displaySerializer(&Serial);
	VirtualDisplay displayProvider(&displaySerializer, 480, 272);
	KemperRemoteDisplay display(kemper, &kemperRemote, &displayProvider);

	SerialDisplaySerializer serialDisplaySerializer("COM7");
	VirtualDisplay displayProvider2(&serialDisplaySerializer, 480, 320);
	KemperRemoteDisplay display2(kemper, &kemperRemote, &displayProvider2);


	//kemperRemote.read();
	//kemper.setRig(2);
	char rigName[20] = "";

	while (true) {
		kemper->read();
		kemperRemote.read();

		if (strcmp(kemper->state.rigName, rigName)) {
			memcpy(rigName, kemper->state.rigName, 20);
		}

		int p1 = 0;
		int p2 = 0;
		int p3 = 0;

		p1 = getc(stdin);
		if (p1 >= 0) {
			
			while ((p2 = getc(stdin)) == -1);
			while ((p3 = getc(stdin)) == -1);

			int mode = p1; 

			if (mode == 1) {
				int sw = p2;
				if (sw > 0) {
					sw--;
					int pressed = p3;
					if (sw < 100)
					{
						if (pressed)
							kemperRemote.onSwitchDown(sw);
						else
							kemperRemote.onSwitchUp(sw);
					}
					else if (sw >= 100 && sw < 200)
					{
						if (pressed) {
							debug("Toggle stomp");
							debug(sw - 100);
							kemper->toggleStomp(sw - 100);
						}
					}
					else if (sw >= 200 && sw < 300)
					{
						if (pressed) {
							if (sw == 200) {
								if (kemper->state.mode == MODE_BROWSE) {
									debug("Perform mode");
									kemper->state.mode = MODE_PERFORM;
									kemper->setPerformance(kemper->state.performance, kemper->state.slot);
								}
								else if (kemper->state.mode == MODE_PERFORM) {
									debug("Browse mode");
									kemper->state.mode = MODE_BROWSE;
									kemper->setRig(kemper->state.currentRig);
								}
							}
						}
					}
				}
			}
			else if (mode == 2) {
				int expId = 0;
			
				expId = p2;

				if (expId>0 && expId <= EXPRESSION_PEDAL_COUNT)
				{
					int expValue = 0;
					expValue = p3;
					if (expValue>=0) {
						kemperRemote.expPedals[expId-1].simValue = expValue<<2;
					}
				}
			}
		}

		fflush(stdout);

		Serial.write(0xF0);
		for (int i=0;i<LED_COUNT*3;i++) {
			Serial.write(kemperRemote.leds[i]);
		}
		Serial.write(0xFF);
		Serial.flush();

		display.draw();
		display2.draw();
		//Sleep(50);
	}


}
