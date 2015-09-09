

#include "Stomp.h"

USING_NAMESPACE_KEMPER

#define ColorNone {0xCC, 0xCC, 0xCC}
#define ColorWhite {0xFF, 0xFF, 0xFF}
#define ColorOrange {0xFF, 0xA5, 0x00}
#define ColorYellow {0xFF, 0xFF, 0x00}
#define ColorCyan {0x00, 0xFF, 0xFF}
#define ColorBlue {0x00, 0x00, 0xFF}
#define ColorPurple {0x80, 0x00, 0x80}
#define ColorGreen {0x00, 0xFF, 0x00}
#define ColorLightGreen {0xA0, 0xFF, 0xA0}
#define ColorRed {0xFF, 0x00, 0x00}

const StompInfo KEMPER_NAMESPACE::AllStomps[] PROGMEM = {
	{1, ColorOrange,"Wah Wah"}, 
	{2, ColorOrange,"Wah Low Pass"},
	{3, ColorOrange,"Wah High Pass"},
	{4, ColorOrange,"Wah Vowel Filter"},
	{6, ColorOrange,"Wah Phaser"},
	{7, ColorOrange,"Wah Flanger"},
	{8, ColorOrange,"Wah Rate Reducer"},
	{9, ColorOrange,"Wah Ring Modulator"},
	{10, ColorOrange,"Wah Freq Shifter"},
	{11, ColorOrange,"Pedal Pitch"},
	{12, ColorOrange,"Wah Formant Shift"},
	{13, ColorLightGreen,"Pedal Vinyl Stop"},
	{17, ColorRed,"Bit Shaper"},
	{18, ColorRed,"Recti Shaper"},
	{19, ColorRed,"Soft Shaper"},
	{20, ColorRed,"Hard Shaper"},
	{21, ColorRed,"Wave Shaper"},
	{33, ColorRed,"Green Screem"},
	{34, ColorRed,"Plus DS"},
	{35, ColorRed,"One DS"},
	{36, ColorRed,"Muffin"},
	{37, ColorRed,"Mouse"},
	{38, ColorRed,"Fuzz DS"},
	{39, ColorRed,"Metal DS"},
	{49, ColorCyan,"Compressor"},
	{57, ColorCyan,"Gate 2:1"},
	{58, ColorCyan,"Gate 4:1"},
	{64, ColorGreen,"Space"},
	{65, ColorBlue,"Vintage Chorus"},
	{66, ColorBlue,"Hyper Chorus"},
	{67, ColorBlue,"Air Chorus"},
	{68, ColorBlue,"Vibrato"},
	{69, ColorBlue,"Rotary Speaker"},
	{70, ColorBlue,"Tremolo"},
	{71, ColorBlue,"Micro Pitch"},
	{81, ColorBlue,"Phaser"},
	{82, ColorBlue,"Phase Vibe"},
	{83, ColorBlue,"Phaser Oneway"},
	{89, ColorBlue,"Flanger"},
	{91, ColorBlue,"Flanger Oneway"},
	{97, ColorYellow,"Graphic Equalizer"},
	{98, ColorYellow,"Studio Equalizer"},
	{99, ColorYellow,"Metal Equalizer"},
	{101, ColorYellow,"Stereo Widener"},
	{113, ColorRed,"Treble Booster"},
	{114, ColorRed,"Lead Booster"},
	{115, ColorRed,"Pure Booster"},
	{116, ColorRed,"Wah Pedal Booster"},
	{121, ColorWhite,"Loop Mono"},
	{122, ColorWhite,"Loop Stereo"},
	{123, ColorWhite,"Loop Distortion"},
	{129, ColorLightGreen,"Transpose"},
	{130, ColorLightGreen,"Chromatic Pitch"},
	{131, ColorLightGreen,"Harmonic Pitch"},
	{132, ColorLightGreen,"Analog Octaver"},
	{601, ColorGreen,"Tap Delay"},
	{602, ColorGreen,"Free Delay"},
	{603, ColorGreen,"Analog Delay"},
	{701, ColorGreen,"Hall"},
	{702, ColorGreen,"Large Room"},
	{703, ColorGreen,"Small Room"},
	{704, ColorGreen,"Ambience"},
	{705, ColorGreen,"Matchbox"}, 
	{0, ColorNone,"None"}, 
};

bool KEMPER_NAMESPACE::getStompInfo(int stompId, int id, StompInfo *info) {
	int indexOffset = stompId >= 6?100*stompId:0;
	for (int i=0;i<AllStompsCount;i++) {
		memcpy_P(info, &(AllStomps[i]), sizeof(StompInfo));
		if (info->type == id + indexOffset)
			return true;
	}
	return false;
}