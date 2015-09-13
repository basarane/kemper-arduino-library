

#pragma once


#include "Arduino.h"

#include "Kemper_namespace.h"

BEGIN_KEMPER_NAMESPACE

struct Color
{
	byte r;
	byte g;
	byte b;
};

struct KemperParamOption
{
	int value;
	char name[20];
};

struct PGM_KemperParam
{
	int number;
	char name[32];
	int minValue;
	int maxValue;
	int optionCount;
	const KemperParamOption* const* options;
};

struct PGM_StompInfo
{
	int type;
	Color color;
	char name[20];
	int paramCount;
	const PGM_KemperParam* const* params;
};

struct KemperParam
{
	int number;
	char name[32];
	int minValue;
	int maxValue;
	int optionCount;
};

struct StompInfo
{
	unsigned int PGM_index;
	int type;
	Color color;
	char name[20];
	int paramCount;
};

struct StompState
{
	bool active;
	bool dirty;
	StompInfo info;
};


#define NUMBER_OF_PARAMS_IN_LIST 5
#define NUMBER_OF_OPTIONS_IN_LIST 5

struct PartialParameter
{
	int paramCount;
	int currentParam;
	int totalParamCount;
	int startParamIndex;

	int optionCount;
	int currentOption;
	int currentValue;
	int totalOptionCount;
	int startOptionIndex;
	bool isActive;

	StompInfo* stompInfo;
	int stompIdx;
	int stompType;
	KemperParam params[NUMBER_OF_PARAMS_IN_LIST];
	KemperParamOption options[NUMBER_OF_PARAMS_IN_LIST];
};

#define AllStompsCount 64

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


// Kemper Parameter Option Values
const KemperParamOption Option_wpm_0 PROGMEM = {0,"Off"};
const KemperParamOption Option_wpm_1 PROGMEM = {1,"Touch"};
const KemperParamOption Option_wpm_2 PROGMEM = {2,"On"};
const KemperParamOption Option_wpm_3 PROGMEM = {3,"Bypass @ Stop"};
const KemperParamOption Option_wpm_4 PROGMEM = {4,"Bypass @ Heel"};
const KemperParamOption Option_wpm_5 PROGMEM = {5,"Bypass @ Toe"};
const KemperParamOption Option_pitch_28 PROGMEM = {28,"-36"};
const KemperParamOption Option_pitch_29 PROGMEM = {29,"-35"};
const KemperParamOption Option_pitch_30 PROGMEM = {30,"-34"};
const KemperParamOption Option_pitch_31 PROGMEM = {31,"-33"};
const KemperParamOption Option_pitch_32 PROGMEM = {32,"-32"};
const KemperParamOption Option_pitch_33 PROGMEM = {33,"-31"};
const KemperParamOption Option_pitch_34 PROGMEM = {34,"-30"};
const KemperParamOption Option_pitch_35 PROGMEM = {35,"-29"};
const KemperParamOption Option_pitch_36 PROGMEM = {36,"-28"};
const KemperParamOption Option_pitch_37 PROGMEM = {37,"-27"};
const KemperParamOption Option_pitch_38 PROGMEM = {38,"-26"};
const KemperParamOption Option_pitch_39 PROGMEM = {39,"-25"};
const KemperParamOption Option_pitch_40 PROGMEM = {40,"-24"};
const KemperParamOption Option_pitch_41 PROGMEM = {41,"-23"};
const KemperParamOption Option_pitch_42 PROGMEM = {42,"-22"};
const KemperParamOption Option_pitch_43 PROGMEM = {43,"-21"};
const KemperParamOption Option_pitch_44 PROGMEM = {44,"-20"};
const KemperParamOption Option_pitch_45 PROGMEM = {45,"-19"};
const KemperParamOption Option_pitch_46 PROGMEM = {46,"-18"};
const KemperParamOption Option_pitch_47 PROGMEM = {47,"-17"};
const KemperParamOption Option_pitch_48 PROGMEM = {48,"-16"};
const KemperParamOption Option_pitch_49 PROGMEM = {49,"-15"};
const KemperParamOption Option_pitch_50 PROGMEM = {50,"-14"};
const KemperParamOption Option_pitch_51 PROGMEM = {51,"-13"};
const KemperParamOption Option_pitch_52 PROGMEM = {52,"-12"};
const KemperParamOption Option_pitch_53 PROGMEM = {53,"-11"};
const KemperParamOption Option_pitch_54 PROGMEM = {54,"-10"};
const KemperParamOption Option_pitch_55 PROGMEM = {55,"-9"};
const KemperParamOption Option_pitch_56 PROGMEM = {56,"-8"};
const KemperParamOption Option_pitch_57 PROGMEM = {57,"-7"};
const KemperParamOption Option_pitch_58 PROGMEM = {58,"-6"};
const KemperParamOption Option_pitch_59 PROGMEM = {59,"-5"};
const KemperParamOption Option_pitch_60 PROGMEM = {60,"-4"};
const KemperParamOption Option_pitch_61 PROGMEM = {61,"-3"};
const KemperParamOption Option_pitch_62 PROGMEM = {62,"-2"};
const KemperParamOption Option_pitch_63 PROGMEM = {63,"-1"};
const KemperParamOption Option_pitch_64 PROGMEM = {64,"0"};
const KemperParamOption Option_pitch_65 PROGMEM = {65,"1"};
const KemperParamOption Option_pitch_66 PROGMEM = {66,"2"};
const KemperParamOption Option_pitch_67 PROGMEM = {67,"3"};
const KemperParamOption Option_pitch_68 PROGMEM = {68,"4"};
const KemperParamOption Option_pitch_69 PROGMEM = {69,"5"};
const KemperParamOption Option_pitch_70 PROGMEM = {70,"6"};
const KemperParamOption Option_pitch_71 PROGMEM = {71,"7"};
const KemperParamOption Option_pitch_72 PROGMEM = {72,"8"};
const KemperParamOption Option_pitch_73 PROGMEM = {73,"9"};
const KemperParamOption Option_pitch_74 PROGMEM = {74,"10"};
const KemperParamOption Option_pitch_75 PROGMEM = {75,"11"};
const KemperParamOption Option_pitch_76 PROGMEM = {76,"12"};
const KemperParamOption Option_pitch_77 PROGMEM = {77,"13"};
const KemperParamOption Option_pitch_78 PROGMEM = {78,"14"};
const KemperParamOption Option_pitch_79 PROGMEM = {79,"15"};
const KemperParamOption Option_pitch_80 PROGMEM = {80,"16"};
const KemperParamOption Option_pitch_81 PROGMEM = {81,"17"};
const KemperParamOption Option_pitch_82 PROGMEM = {82,"18"};
const KemperParamOption Option_pitch_83 PROGMEM = {83,"19"};
const KemperParamOption Option_pitch_84 PROGMEM = {84,"20"};
const KemperParamOption Option_pitch_85 PROGMEM = {85,"21"};
const KemperParamOption Option_pitch_86 PROGMEM = {86,"22"};
const KemperParamOption Option_pitch_87 PROGMEM = {87,"23"};
const KemperParamOption Option_pitch_88 PROGMEM = {88,"24"};
const KemperParamOption Option_pitch_89 PROGMEM = {89,"25"};
const KemperParamOption Option_pitch_90 PROGMEM = {90,"26"};
const KemperParamOption Option_pitch_91 PROGMEM = {91,"27"};
const KemperParamOption Option_pitch_92 PROGMEM = {92,"28"};
const KemperParamOption Option_pitch_93 PROGMEM = {93,"29"};
const KemperParamOption Option_pitch_94 PROGMEM = {94,"30"};
const KemperParamOption Option_pitch_95 PROGMEM = {95,"31"};
const KemperParamOption Option_pitch_96 PROGMEM = {96,"32"};
const KemperParamOption Option_pitch_97 PROGMEM = {97,"33"};
const KemperParamOption Option_pitch_98 PROGMEM = {98,"34"};
const KemperParamOption Option_pitch_99 PROGMEM = {99,"35"};
const KemperParamOption Option_pitch_100 PROGMEM = {100,"36"};
const KemperParamOption Option_modrate_0 PROGMEM = {0,"0"};
const KemperParamOption Option_modrate_1 PROGMEM = {1,"1"};
const KemperParamOption Option_modrate_2 PROGMEM = {2,"2"};
const KemperParamOption Option_modrate_3 PROGMEM = {3,"3"};
const KemperParamOption Option_modrate_4 PROGMEM = {4,"4"};
const KemperParamOption Option_modrate_5 PROGMEM = {5,"5"};
const KemperParamOption Option_modrate_6 PROGMEM = {6,"6"};
const KemperParamOption Option_modrate_7 PROGMEM = {7,"7"};
const KemperParamOption Option_modrate_8 PROGMEM = {8,"8"};
const KemperParamOption Option_modrate_9 PROGMEM = {9,"9"};
const KemperParamOption Option_modrate_10 PROGMEM = {10,"10"};
const KemperParamOption Option_modrate_11 PROGMEM = {11,"11"};
const KemperParamOption Option_modrate_12 PROGMEM = {12,"12"};
const KemperParamOption Option_modrate_13 PROGMEM = {13,"13"};
const KemperParamOption Option_modrate_14 PROGMEM = {14,"14"};
const KemperParamOption Option_modrate_15 PROGMEM = {15,"15"};
const KemperParamOption Option_modrate_16 PROGMEM = {16,"16"};
const KemperParamOption Option_modrate_17 PROGMEM = {17,"17"};
const KemperParamOption Option_modrate_18 PROGMEM = {18,"18"};
const KemperParamOption Option_modrate_19 PROGMEM = {19,"19"};
const KemperParamOption Option_modrate_20 PROGMEM = {20,"20"};
const KemperParamOption Option_modrate_21 PROGMEM = {21,"21"};
const KemperParamOption Option_modrate_22 PROGMEM = {22,"22"};
const KemperParamOption Option_modrate_23 PROGMEM = {23,"23"};
const KemperParamOption Option_modrate_24 PROGMEM = {24,"24"};
const KemperParamOption Option_modrate_25 PROGMEM = {25,"25"};
const KemperParamOption Option_modrate_26 PROGMEM = {26,"26"};
const KemperParamOption Option_modrate_27 PROGMEM = {27,"27"};
const KemperParamOption Option_modrate_28 PROGMEM = {28,"28"};
const KemperParamOption Option_modrate_29 PROGMEM = {29,"29"};
const KemperParamOption Option_modrate_30 PROGMEM = {30,"30"};
const KemperParamOption Option_modrate_31 PROGMEM = {31,"31"};
const KemperParamOption Option_modrate_32 PROGMEM = {32,"32"};
const KemperParamOption Option_modrate_33 PROGMEM = {33,"33"};
const KemperParamOption Option_modrate_34 PROGMEM = {34,"34"};
const KemperParamOption Option_modrate_35 PROGMEM = {35,"35"};
const KemperParamOption Option_modrate_36 PROGMEM = {36,"36"};
const KemperParamOption Option_modrate_37 PROGMEM = {37,"37"};
const KemperParamOption Option_modrate_38 PROGMEM = {38,"38"};
const KemperParamOption Option_modrate_39 PROGMEM = {39,"39"};
const KemperParamOption Option_modrate_40 PROGMEM = {40,"40"};
const KemperParamOption Option_modrate_41 PROGMEM = {41,"41"};
const KemperParamOption Option_modrate_42 PROGMEM = {42,"42"};
const KemperParamOption Option_modrate_43 PROGMEM = {43,"43"};
const KemperParamOption Option_modrate_44 PROGMEM = {44,"44"};
const KemperParamOption Option_modrate_45 PROGMEM = {45,"45"};
const KemperParamOption Option_modrate_46 PROGMEM = {46,"46"};
const KemperParamOption Option_modrate_47 PROGMEM = {47,"47"};
const KemperParamOption Option_modrate_48 PROGMEM = {48,"48"};
const KemperParamOption Option_modrate_49 PROGMEM = {49,"49"};
const KemperParamOption Option_modrate_50 PROGMEM = {50,"50"};
const KemperParamOption Option_modrate_51 PROGMEM = {51,"51"};
const KemperParamOption Option_modrate_52 PROGMEM = {52,"52"};
const KemperParamOption Option_modrate_53 PROGMEM = {53,"53"};
const KemperParamOption Option_modrate_54 PROGMEM = {54,"54"};
const KemperParamOption Option_modrate_55 PROGMEM = {55,"55"};
const KemperParamOption Option_modrate_56 PROGMEM = {56,"56"};
const KemperParamOption Option_modrate_57 PROGMEM = {57,"57"};
const KemperParamOption Option_modrate_58 PROGMEM = {58,"58"};
const KemperParamOption Option_modrate_59 PROGMEM = {59,"59"};
const KemperParamOption Option_modrate_60 PROGMEM = {60,"60"};
const KemperParamOption Option_modrate_61 PROGMEM = {61,"61"};
const KemperParamOption Option_modrate_62 PROGMEM = {62,"62"};
const KemperParamOption Option_modrate_63 PROGMEM = {63,"63"};
const KemperParamOption Option_modrate_64 PROGMEM = {64,"64"};
const KemperParamOption Option_modrate_65 PROGMEM = {65,"65"};
const KemperParamOption Option_modrate_66 PROGMEM = {66,"66"};
const KemperParamOption Option_modrate_67 PROGMEM = {67,"67"};
const KemperParamOption Option_modrate_68 PROGMEM = {68,"68"};
const KemperParamOption Option_modrate_69 PROGMEM = {69,"69"};
const KemperParamOption Option_modrate_70 PROGMEM = {70,"70"};
const KemperParamOption Option_modrate_71 PROGMEM = {71,"71"};
const KemperParamOption Option_modrate_72 PROGMEM = {72,"72"};
const KemperParamOption Option_modrate_73 PROGMEM = {73,"73"};
const KemperParamOption Option_modrate_74 PROGMEM = {74,"74"};
const KemperParamOption Option_modrate_75 PROGMEM = {75,"75"};
const KemperParamOption Option_modrate_76 PROGMEM = {76,"76"};
const KemperParamOption Option_modrate_77 PROGMEM = {77,"77"};
const KemperParamOption Option_modrate_78 PROGMEM = {78,"78"};
const KemperParamOption Option_modrate_79 PROGMEM = {79,"79"};
const KemperParamOption Option_modrate_80 PROGMEM = {80,"80"};
const KemperParamOption Option_modrate_81 PROGMEM = {81,"81"};
const KemperParamOption Option_modrate_82 PROGMEM = {82,"82"};
const KemperParamOption Option_modrate_83 PROGMEM = {83,"83"};
const KemperParamOption Option_modrate_84 PROGMEM = {84,"84"};
const KemperParamOption Option_modrate_85 PROGMEM = {85,"85"};
const KemperParamOption Option_modrate_86 PROGMEM = {86,"86"};
const KemperParamOption Option_modrate_87 PROGMEM = {87,"87"};
const KemperParamOption Option_modrate_88 PROGMEM = {88,"88"};
const KemperParamOption Option_modrate_89 PROGMEM = {89,"89"};
const KemperParamOption Option_modrate_90 PROGMEM = {90,"90"};
const KemperParamOption Option_modrate_91 PROGMEM = {91,"91"};
const KemperParamOption Option_modrate_92 PROGMEM = {92,"92"};
const KemperParamOption Option_modrate_93 PROGMEM = {93,"93"};
const KemperParamOption Option_modrate_94 PROGMEM = {94,"94"};
const KemperParamOption Option_modrate_95 PROGMEM = {95,"95"};
const KemperParamOption Option_modrate_96 PROGMEM = {96,"96"};
const KemperParamOption Option_modrate_97 PROGMEM = {97,"97"};
const KemperParamOption Option_modrate_98 PROGMEM = {98,"98"};
const KemperParamOption Option_modrate_99 PROGMEM = {99,"99"};
const KemperParamOption Option_modrate_100 PROGMEM = {100,"100"};
const KemperParamOption Option_modrate_101 PROGMEM = {101,"101"};
const KemperParamOption Option_modrate_102 PROGMEM = {102,"102"};
const KemperParamOption Option_modrate_103 PROGMEM = {103,"103"};
const KemperParamOption Option_modrate_104 PROGMEM = {104,"104"};
const KemperParamOption Option_modrate_105 PROGMEM = {105,"105"};
const KemperParamOption Option_modrate_106 PROGMEM = {106,"106"};
const KemperParamOption Option_modrate_107 PROGMEM = {107,"107"};
const KemperParamOption Option_modrate_108 PROGMEM = {108,"108"};
const KemperParamOption Option_modrate_109 PROGMEM = {109,"109"};
const KemperParamOption Option_modrate_110 PROGMEM = {110,"110"};
const KemperParamOption Option_modrate_111 PROGMEM = {111,"111"};
const KemperParamOption Option_modrate_112 PROGMEM = {112,"112"};
const KemperParamOption Option_modrate_113 PROGMEM = {113,"113"};
const KemperParamOption Option_modrate_114 PROGMEM = {114,"114"};
const KemperParamOption Option_modrate_115 PROGMEM = {115,"115"};
const KemperParamOption Option_modrate_116 PROGMEM = {116,"116"};
const KemperParamOption Option_modrate_117 PROGMEM = {117,"117"};
const KemperParamOption Option_modrate_118 PROGMEM = {118,"118"};
const KemperParamOption Option_modrate_119 PROGMEM = {119,"119"};
const KemperParamOption Option_modrate_120 PROGMEM = {120,"120"};
const KemperParamOption Option_modrate_121 PROGMEM = {121,"121"};
const KemperParamOption Option_modrate_122 PROGMEM = {122,"122"};
const KemperParamOption Option_modrate_123 PROGMEM = {123,"123"};
const KemperParamOption Option_modrate_124 PROGMEM = {124,"124"};
const KemperParamOption Option_modrate_125 PROGMEM = {125,"125"};
const KemperParamOption Option_modrate_126 PROGMEM = {126,"126"};
const KemperParamOption Option_modrate_127 PROGMEM = {127,"127"};
const KemperParamOption Option_key_0 PROGMEM = {0,"C/Am"};
const KemperParamOption Option_key_1 PROGMEM = {1,"Db/Bbm"};
const KemperParamOption Option_key_2 PROGMEM = {2,"D/Bm"};
const KemperParamOption Option_key_3 PROGMEM = {3,"Eb/Cm"};
const KemperParamOption Option_key_4 PROGMEM = {4,"E/C#m"};
const KemperParamOption Option_key_5 PROGMEM = {5,"F/Dm"};
const KemperParamOption Option_key_6 PROGMEM = {6,"F#/D#m"};
const KemperParamOption Option_key_7 PROGMEM = {7,"G/Em"};
const KemperParamOption Option_key_8 PROGMEM = {8,"Ab/Fm"};
const KemperParamOption Option_key_9 PROGMEM = {9,"A/F#m"};
const KemperParamOption Option_key_10 PROGMEM = {10,"Bb/Gm"};
const KemperParamOption Option_key_11 PROGMEM = {11,"B/G#m"};
const KemperParamOption Option_modstages_0 PROGMEM = {0,"Stages 2"};
const KemperParamOption Option_modstages_1 PROGMEM = {1,"Stages 4"};
const KemperParamOption Option_modstages_2 PROGMEM = {2,"Stages 6"};
const KemperParamOption Option_modstages_3 PROGMEM = {3,"Stages 8"};
const KemperParamOption Option_modstages_4 PROGMEM = {4,"Stages 10"};
const KemperParamOption Option_modstages_5 PROGMEM = {5,"Stages 12"};
const KemperParamOption Option_voiceinterval_42 PROGMEM = {42,"-3 Oct"};
const KemperParamOption Option_voiceinterval_43 PROGMEM = {43,"-21th"};
const KemperParamOption Option_voiceinterval_44 PROGMEM = {44,"-20th"};
const KemperParamOption Option_voiceinterval_45 PROGMEM = {45,"-19th"};
const KemperParamOption Option_voiceinterval_46 PROGMEM = {46,"-18th"};
const KemperParamOption Option_voiceinterval_47 PROGMEM = {47,"-17th"};
const KemperParamOption Option_voiceinterval_48 PROGMEM = {48,"-16th"};
const KemperParamOption Option_voiceinterval_49 PROGMEM = {49,"-2 Oct"};
const KemperParamOption Option_voiceinterval_50 PROGMEM = {50,"-14th"};
const KemperParamOption Option_voiceinterval_51 PROGMEM = {51,"-13th"};
const KemperParamOption Option_voiceinterval_52 PROGMEM = {52,"-12th"};
const KemperParamOption Option_voiceinterval_53 PROGMEM = {53,"-11th"};
const KemperParamOption Option_voiceinterval_54 PROGMEM = {54,"-10th"};
const KemperParamOption Option_voiceinterval_55 PROGMEM = {55,"-9th"};
const KemperParamOption Option_voiceinterval_56 PROGMEM = {56,"-8th"};
const KemperParamOption Option_voiceinterval_57 PROGMEM = {57,"-1 Oct"};
const KemperParamOption Option_voiceinterval_58 PROGMEM = {58,"-7th"};
const KemperParamOption Option_voiceinterval_59 PROGMEM = {59,"-6th"};
const KemperParamOption Option_voiceinterval_60 PROGMEM = {60,"-5th"};
const KemperParamOption Option_voiceinterval_61 PROGMEM = {61,"-4th"};
const KemperParamOption Option_voiceinterval_62 PROGMEM = {62,"-3rd"};
const KemperParamOption Option_voiceinterval_63 PROGMEM = {63,"-2nd"};
const KemperParamOption Option_voiceinterval_64 PROGMEM = {64,"Unision"};
const KemperParamOption Option_voiceinterval_65 PROGMEM = {65,"+2nd"};
const KemperParamOption Option_voiceinterval_66 PROGMEM = {66,"+3rd"};
const KemperParamOption Option_voiceinterval_67 PROGMEM = {67,"+4th"};
const KemperParamOption Option_voiceinterval_68 PROGMEM = {68,"+5th"};
const KemperParamOption Option_voiceinterval_69 PROGMEM = {69,"+6th"};
const KemperParamOption Option_voiceinterval_70 PROGMEM = {70,"+7th"};
const KemperParamOption Option_voiceinterval_71 PROGMEM = {71,"+1 Oct"};
const KemperParamOption Option_voiceinterval_72 PROGMEM = {72,"+9th"};
const KemperParamOption Option_voiceinterval_73 PROGMEM = {73,"+10th"};
const KemperParamOption Option_voiceinterval_74 PROGMEM = {74,"+11th"};
const KemperParamOption Option_voiceinterval_75 PROGMEM = {75,"+12th"};
const KemperParamOption Option_voiceinterval_76 PROGMEM = {76,"+13th"};
const KemperParamOption Option_voiceinterval_77 PROGMEM = {77,"+14th"};
const KemperParamOption Option_voiceinterval_78 PROGMEM = {78,"+2 Oct"};
const KemperParamOption Option_voiceinterval_79 PROGMEM = {79,"+16th"};
const KemperParamOption Option_voiceinterval_80 PROGMEM = {80,"+17th"};
const KemperParamOption Option_voiceinterval_81 PROGMEM = {81,"+18th"};
const KemperParamOption Option_voiceinterval_82 PROGMEM = {82,"+19th"};
const KemperParamOption Option_voiceinterval_83 PROGMEM = {83,"+20th"};
const KemperParamOption Option_voiceinterval_84 PROGMEM = {84,"+21th"};
const KemperParamOption Option_voiceinterval_85 PROGMEM = {85,"+3 Oct"};
const KemperParamOption Option_voiceinterval_86 PROGMEM = {86,"User Scale1 -2 Oct"};
const KemperParamOption Option_voiceinterval_87 PROGMEM = {87,"User Scale1 -1 Oct"};
const KemperParamOption Option_voiceinterval_88 PROGMEM = {88,"User Scale1"};
const KemperParamOption Option_voiceinterval_89 PROGMEM = {89,"User Scale1 +1 Oct"};
const KemperParamOption Option_voiceinterval_90 PROGMEM = {90,"User Scale1 +2 Oct"};
const KemperParamOption Option_voiceinterval_91 PROGMEM = {91,"User Scale2 -2 Oct"};
const KemperParamOption Option_voiceinterval_92 PROGMEM = {92,"User Scale2 -1 Oct"};
const KemperParamOption Option_voiceinterval_93 PROGMEM = {93,"User Scale2"};
const KemperParamOption Option_voiceinterval_94 PROGMEM = {94,"User Scale2 +1 Oct"};
const KemperParamOption Option_voiceinterval_95 PROGMEM = {95,"User Scale2 +2 Oct"};
const KemperParamOption Option_t_1 PROGMEM = {1,"Wah Wah"};
const KemperParamOption Option_t_2 PROGMEM = {2,"Wah Low Pass"};
const KemperParamOption Option_t_3 PROGMEM = {3,"Wah High Pass"};
const KemperParamOption Option_t_4 PROGMEM = {4,"Wah Vowel Filter"};
const KemperParamOption Option_t_6 PROGMEM = {6,"Wah Phaser"};
const KemperParamOption Option_t_7 PROGMEM = {7,"Wah Flanger"};
const KemperParamOption Option_t_8 PROGMEM = {8,"Wah Rate Reducer"};
const KemperParamOption Option_t_9 PROGMEM = {9,"Wah Ring Modulator"};
const KemperParamOption Option_t_10 PROGMEM = {10,"Wah Freq Shifter"};
const KemperParamOption Option_t_11 PROGMEM = {11,"Pedal Pitch"};
const KemperParamOption Option_t_12 PROGMEM = {12,"Wah Formant Shift"};
const KemperParamOption Option_t_13 PROGMEM = {13,"Pedal Vinyl Stop"};
const KemperParamOption Option_t_17 PROGMEM = {17,"Bit Shaper"};
const KemperParamOption Option_t_18 PROGMEM = {18,"Recti Shaper"};
const KemperParamOption Option_t_19 PROGMEM = {19,"Soft Shaper"};
const KemperParamOption Option_t_20 PROGMEM = {20,"Hard Shaper"};
const KemperParamOption Option_t_21 PROGMEM = {21,"Wave Shaper"};
const KemperParamOption Option_t_33 PROGMEM = {33,"Green Screem"};
const KemperParamOption Option_t_34 PROGMEM = {34,"Plus DS"};
const KemperParamOption Option_t_35 PROGMEM = {35,"One DS"};
const KemperParamOption Option_t_36 PROGMEM = {36,"Muffin"};
const KemperParamOption Option_t_37 PROGMEM = {37,"Mouse"};
const KemperParamOption Option_t_38 PROGMEM = {38,"Fuzz DS"};
const KemperParamOption Option_t_39 PROGMEM = {39,"Metal DS"};
const KemperParamOption Option_t_49 PROGMEM = {49,"Compressor"};
const KemperParamOption Option_t_57 PROGMEM = {57,"Gate 2:1"};
const KemperParamOption Option_t_58 PROGMEM = {58,"Gate 4:1"};
const KemperParamOption Option_t_64 PROGMEM = {64,"Space"};
const KemperParamOption Option_t_65 PROGMEM = {65,"Vintage Chorus"};
const KemperParamOption Option_t_66 PROGMEM = {66,"Hyper Chorus"};
const KemperParamOption Option_t_67 PROGMEM = {67,"Air Chorus"};
const KemperParamOption Option_t_68 PROGMEM = {68,"Vibrato"};
const KemperParamOption Option_t_69 PROGMEM = {69,"Rotary Speaker"};
const KemperParamOption Option_t_70 PROGMEM = {70,"Tremolo"};
const KemperParamOption Option_t_71 PROGMEM = {71,"Micro Pitch"};
const KemperParamOption Option_t_81 PROGMEM = {81,"Phaser"};
const KemperParamOption Option_t_82 PROGMEM = {82,"Phase Vibe"};
const KemperParamOption Option_t_83 PROGMEM = {83,"Phaser Oneway"};
const KemperParamOption Option_t_89 PROGMEM = {89,"Flanger"};
const KemperParamOption Option_t_91 PROGMEM = {91,"Flanger Oneway"};
const KemperParamOption Option_t_97 PROGMEM = {97,"Graphic Equalizer"};
const KemperParamOption Option_t_98 PROGMEM = {98,"Studio Equalizer"};
const KemperParamOption Option_t_99 PROGMEM = {99,"Metal Equalizer"};
const KemperParamOption Option_t_101 PROGMEM = {101,"Stereo Widener"};
const KemperParamOption Option_t_113 PROGMEM = {113,"Treble Booster"};
const KemperParamOption Option_t_114 PROGMEM = {114,"Lead Booster"};
const KemperParamOption Option_t_115 PROGMEM = {115,"Pure Booster"};
const KemperParamOption Option_t_116 PROGMEM = {116,"Wah Pedal Booster"};
const KemperParamOption Option_t_121 PROGMEM = {121,"Loop Mono"};
const KemperParamOption Option_t_122 PROGMEM = {122,"Loop Stereo"};
const KemperParamOption Option_t_123 PROGMEM = {123,"Loop Distortion"};
const KemperParamOption Option_t_129 PROGMEM = {129,"Transpose"};
const KemperParamOption Option_t_130 PROGMEM = {130,"Chromatic Pitch"};
const KemperParamOption Option_t_131 PROGMEM = {131,"Harmonic Pitch"};
const KemperParamOption Option_t_132 PROGMEM = {132,"Analog Octaver"};
const KemperParamOption Option_delayratio_0 PROGMEM = {0,"1:4"};
const KemperParamOption Option_delayratio_1 PROGMEM = {1,"2:4"};
const KemperParamOption Option_delayratio_2 PROGMEM = {2,"3:4"};
const KemperParamOption Option_delayratio_3 PROGMEM = {3,"4:4"};
const KemperParamOption Option_delayratio_4 PROGMEM = {4,"4:3"};
const KemperParamOption Option_delayratio_5 PROGMEM = {5,"4:2"};
const KemperParamOption Option_delayratio_6 PROGMEM = {6,"4:1"};
const KemperParamOption Option_delaytype_0 PROGMEM = {0,""};
const KemperParamOption Option_delaytype_1 PROGMEM = {1,"Tap Delay"};
const KemperParamOption Option_delaytype_2 PROGMEM = {2,"Free Delay"};
const KemperParamOption Option_delaytype_3 PROGMEM = {3,"Analog Delay"};
const KemperParamOption Option_delayclock_0 PROGMEM = {0,"1/32"};
const KemperParamOption Option_delayclock_1 PROGMEM = {1,"1/16"};
const KemperParamOption Option_delayclock_2 PROGMEM = {2,"2/16"};
const KemperParamOption Option_delayclock_3 PROGMEM = {3,"3/16"};
const KemperParamOption Option_delayclock_4 PROGMEM = {4,"4/16"};
const KemperParamOption Option_delayclock_5 PROGMEM = {5,"5/16"};
const KemperParamOption Option_delayclock_6 PROGMEM = {6,"6/16"};
const KemperParamOption Option_delayclock_7 PROGMEM = {7,"7/16"};
const KemperParamOption Option_delayclock_8 PROGMEM = {8,"8/16"};
const KemperParamOption Option_delayclock_9 PROGMEM = {9,"1/24(Triplet)"};
const KemperParamOption Option_delayclock_10 PROGMEM = {10,"1/12(Triplet)"};
const KemperParamOption Option_delayclock_11 PROGMEM = {11,"2/12(Triplet)"};
const KemperParamOption Option_delayclock_12 PROGMEM = {12,"4/12(Triplet)"};
const KemperParamOption Option_b_0 PROGMEM = {0,"Off"};
const KemperParamOption Option_b_1 PROGMEM = {1,"On"};
const KemperParamOption Option_reverbtype_0 PROGMEM = {0,""};
const KemperParamOption Option_reverbtype_1 PROGMEM = {1,"Hall"};
const KemperParamOption Option_reverbtype_2 PROGMEM = {2,"Large Room"};
const KemperParamOption Option_reverbtype_3 PROGMEM = {3,"Small Room"};
const KemperParamOption Option_reverbtype_4 PROGMEM = {4,"Ambience"};
const KemperParamOption Option_reverbtype_5 PROGMEM = {5,"Matchbox"};

// Kemper Parameter Option Collections
const KemperParamOption * const Options_wpm[] PROGMEM = {&Option_wpm_0, &Option_wpm_1, &Option_wpm_2, &Option_wpm_3, &Option_wpm_4, &Option_wpm_5};
const KemperParamOption * const Options_pitch[] PROGMEM = {&Option_pitch_28, &Option_pitch_29, &Option_pitch_30, &Option_pitch_31, &Option_pitch_32, &Option_pitch_33, &Option_pitch_34, &Option_pitch_35, &Option_pitch_36, &Option_pitch_37, &Option_pitch_38, &Option_pitch_39, &Option_pitch_40, &Option_pitch_41, &Option_pitch_42, &Option_pitch_43, &Option_pitch_44, &Option_pitch_45, &Option_pitch_46, &Option_pitch_47, &Option_pitch_48, &Option_pitch_49, &Option_pitch_50, &Option_pitch_51, &Option_pitch_52, &Option_pitch_53, &Option_pitch_54, &Option_pitch_55, &Option_pitch_56, &Option_pitch_57, &Option_pitch_58, &Option_pitch_59, &Option_pitch_60, &Option_pitch_61, &Option_pitch_62, &Option_pitch_63, &Option_pitch_64, &Option_pitch_65, &Option_pitch_66, &Option_pitch_67, &Option_pitch_68, &Option_pitch_69, &Option_pitch_70, &Option_pitch_71, &Option_pitch_72, &Option_pitch_73, &Option_pitch_74, &Option_pitch_75, &Option_pitch_76, &Option_pitch_77, &Option_pitch_78, &Option_pitch_79, &Option_pitch_80, &Option_pitch_81, &Option_pitch_82, &Option_pitch_83, &Option_pitch_84, &Option_pitch_85, &Option_pitch_86, &Option_pitch_87, &Option_pitch_88, &Option_pitch_89, &Option_pitch_90, &Option_pitch_91, &Option_pitch_92, &Option_pitch_93, &Option_pitch_94, &Option_pitch_95, &Option_pitch_96, &Option_pitch_97, &Option_pitch_98, &Option_pitch_99, &Option_pitch_100};
const KemperParamOption * const Options_modrate[] PROGMEM = {&Option_modrate_0, &Option_modrate_1, &Option_modrate_2, &Option_modrate_3, &Option_modrate_4, &Option_modrate_5, &Option_modrate_6, &Option_modrate_7, &Option_modrate_8, &Option_modrate_9, &Option_modrate_10, &Option_modrate_11, &Option_modrate_12, &Option_modrate_13, &Option_modrate_14, &Option_modrate_15, &Option_modrate_16, &Option_modrate_17, &Option_modrate_18, &Option_modrate_19, &Option_modrate_20, &Option_modrate_21, &Option_modrate_22, &Option_modrate_23, &Option_modrate_24, &Option_modrate_25, &Option_modrate_26, &Option_modrate_27, &Option_modrate_28, &Option_modrate_29, &Option_modrate_30, &Option_modrate_31, &Option_modrate_32, &Option_modrate_33, &Option_modrate_34, &Option_modrate_35, &Option_modrate_36, &Option_modrate_37, &Option_modrate_38, &Option_modrate_39, &Option_modrate_40, &Option_modrate_41, &Option_modrate_42, &Option_modrate_43, &Option_modrate_44, &Option_modrate_45, &Option_modrate_46, &Option_modrate_47, &Option_modrate_48, &Option_modrate_49, &Option_modrate_50, &Option_modrate_51, &Option_modrate_52, &Option_modrate_53, &Option_modrate_54, &Option_modrate_55, &Option_modrate_56, &Option_modrate_57, &Option_modrate_58, &Option_modrate_59, &Option_modrate_60, &Option_modrate_61, &Option_modrate_62, &Option_modrate_63, &Option_modrate_64, &Option_modrate_65, &Option_modrate_66, &Option_modrate_67, &Option_modrate_68, &Option_modrate_69, &Option_modrate_70, &Option_modrate_71, &Option_modrate_72, &Option_modrate_73, &Option_modrate_74, &Option_modrate_75, &Option_modrate_76, &Option_modrate_77, &Option_modrate_78, &Option_modrate_79, &Option_modrate_80, &Option_modrate_81, &Option_modrate_82, &Option_modrate_83, &Option_modrate_84, &Option_modrate_85, &Option_modrate_86, &Option_modrate_87, &Option_modrate_88, &Option_modrate_89, &Option_modrate_90, &Option_modrate_91, &Option_modrate_92, &Option_modrate_93, &Option_modrate_94, &Option_modrate_95, &Option_modrate_96, &Option_modrate_97, &Option_modrate_98, &Option_modrate_99, &Option_modrate_100, &Option_modrate_101, &Option_modrate_102, &Option_modrate_103, &Option_modrate_104, &Option_modrate_105, &Option_modrate_106, &Option_modrate_107, &Option_modrate_108, &Option_modrate_109, &Option_modrate_110, &Option_modrate_111, &Option_modrate_112, &Option_modrate_113, &Option_modrate_114, &Option_modrate_115, &Option_modrate_116, &Option_modrate_117, &Option_modrate_118, &Option_modrate_119, &Option_modrate_120, &Option_modrate_121, &Option_modrate_122, &Option_modrate_123, &Option_modrate_124, &Option_modrate_125, &Option_modrate_126, &Option_modrate_127};
const KemperParamOption * const Options_key[] PROGMEM = {&Option_key_0, &Option_key_1, &Option_key_2, &Option_key_3, &Option_key_4, &Option_key_5, &Option_key_6, &Option_key_7, &Option_key_8, &Option_key_9, &Option_key_10, &Option_key_11};
const KemperParamOption * const Options_modstages[] PROGMEM = {&Option_modstages_0, &Option_modstages_1, &Option_modstages_2, &Option_modstages_3, &Option_modstages_4, &Option_modstages_5};
const KemperParamOption * const Options_voiceinterval[] PROGMEM = {&Option_voiceinterval_42, &Option_voiceinterval_43, &Option_voiceinterval_44, &Option_voiceinterval_45, &Option_voiceinterval_46, &Option_voiceinterval_47, &Option_voiceinterval_48, &Option_voiceinterval_49, &Option_voiceinterval_50, &Option_voiceinterval_51, &Option_voiceinterval_52, &Option_voiceinterval_53, &Option_voiceinterval_54, &Option_voiceinterval_55, &Option_voiceinterval_56, &Option_voiceinterval_57, &Option_voiceinterval_58, &Option_voiceinterval_59, &Option_voiceinterval_60, &Option_voiceinterval_61, &Option_voiceinterval_62, &Option_voiceinterval_63, &Option_voiceinterval_64, &Option_voiceinterval_65, &Option_voiceinterval_66, &Option_voiceinterval_67, &Option_voiceinterval_68, &Option_voiceinterval_69, &Option_voiceinterval_70, &Option_voiceinterval_71, &Option_voiceinterval_72, &Option_voiceinterval_73, &Option_voiceinterval_74, &Option_voiceinterval_75, &Option_voiceinterval_76, &Option_voiceinterval_77, &Option_voiceinterval_78, &Option_voiceinterval_79, &Option_voiceinterval_80, &Option_voiceinterval_81, &Option_voiceinterval_82, &Option_voiceinterval_83, &Option_voiceinterval_84, &Option_voiceinterval_85, &Option_voiceinterval_86, &Option_voiceinterval_87, &Option_voiceinterval_88, &Option_voiceinterval_89, &Option_voiceinterval_90, &Option_voiceinterval_91, &Option_voiceinterval_92, &Option_voiceinterval_93, &Option_voiceinterval_94, &Option_voiceinterval_95};
const KemperParamOption * const Options_t[] PROGMEM = {&Option_t_1, &Option_t_2, &Option_t_3, &Option_t_4, &Option_t_6, &Option_t_7, &Option_t_8, &Option_t_9, &Option_t_10, &Option_t_11, &Option_t_12, &Option_t_13, &Option_t_17, &Option_t_18, &Option_t_19, &Option_t_20, &Option_t_21, &Option_t_33, &Option_t_34, &Option_t_35, &Option_t_36, &Option_t_37, &Option_t_38, &Option_t_39, &Option_t_49, &Option_t_57, &Option_t_58, &Option_t_64, &Option_t_65, &Option_t_66, &Option_t_67, &Option_t_68, &Option_t_69, &Option_t_70, &Option_t_71, &Option_t_81, &Option_t_82, &Option_t_83, &Option_t_89, &Option_t_91, &Option_t_97, &Option_t_98, &Option_t_99, &Option_t_101, &Option_t_113, &Option_t_114, &Option_t_115, &Option_t_116, &Option_t_121, &Option_t_122, &Option_t_123, &Option_t_129, &Option_t_130, &Option_t_131, &Option_t_132};
const KemperParamOption * const Options_delayratio[] PROGMEM = {&Option_delayratio_0, &Option_delayratio_1, &Option_delayratio_2, &Option_delayratio_3, &Option_delayratio_4, &Option_delayratio_5, &Option_delayratio_6};
const KemperParamOption * const Options_delaytype[] PROGMEM = {&Option_delaytype_0, &Option_delaytype_1, &Option_delaytype_2, &Option_delaytype_3};
const KemperParamOption * const Options_delayclock[] PROGMEM = {&Option_delayclock_0, &Option_delayclock_1, &Option_delayclock_2, &Option_delayclock_3, &Option_delayclock_4, &Option_delayclock_5, &Option_delayclock_6, &Option_delayclock_7, &Option_delayclock_8, &Option_delayclock_9, &Option_delayclock_10, &Option_delayclock_11, &Option_delayclock_12};
const KemperParamOption * const Options_b[] PROGMEM = {&Option_b_0, &Option_b_1};
const KemperParamOption * const Options_reverbtype[] PROGMEM = {&Option_reverbtype_0, &Option_reverbtype_1, &Option_reverbtype_2, &Option_reverbtype_3, &Option_reverbtype_4, &Option_reverbtype_5};

// Kemper Parameters
const PGM_KemperParam Param_Type PROGMEM =  {0, "Type", 1, 132, 55, Options_t};
const PGM_KemperParam Param_On_Off PROGMEM =  {3, "On/Off", 0, 1, 2, Options_b};
const PGM_KemperParam Param_Mix PROGMEM =  {4, "Mix", 0, 100, 0, 0};
const PGM_KemperParam Param_Volume PROGMEM =  {6, "Volume", 0, 100, 0, 0};
const PGM_KemperParam Param_Stereo PROGMEM =  {7, "Stereo", 0, 100, 0, 0};
const PGM_KemperParam Param_Wah_Manual PROGMEM =  {8, "Wah Manual", 0, 100, 0, 0};
const PGM_KemperParam Param_Wah_Peak PROGMEM =  {9, "Wah Peak", 0, 100, 0, 0};
const PGM_KemperParam Param_Wah_Range PROGMEM =  {10, "Wah Range", 0, 100, 0, 0};
const PGM_KemperParam Param_Wah_Pedal_Mode PROGMEM =  {12, "Wah Pedal Mode", 0, 5, 6, Options_wpm};
const PGM_KemperParam Param_Wah_Touch_Attack PROGMEM =  {13, "Wah Touch Attack", 0, 100, 0, 0};
const PGM_KemperParam Param_Wah_Touch_Release PROGMEM =  {14, "Wah Touch Release", 0, 100, 0, 0};
const PGM_KemperParam Param_Wah_Touch_Boost PROGMEM =  {15, "Wah Touch Boost", 0, 100, 0, 0};
const PGM_KemperParam Param_Distortion_Shaper_Drive PROGMEM =  {16, "Distortion/Shaper Drive", 0, 100, 0, 0};
const PGM_KemperParam Param_Distortion_Booster_Tone PROGMEM =  {17, "Distortion/Booster Tone", 0, 100, 0, 0};
const PGM_KemperParam Param_Compressor_Gate_Intensity PROGMEM =  {18, "Compressor/Gate Intensity", 0, 100, 0, 0};
const PGM_KemperParam Param_Compressor_Attack PROGMEM =  {19, "Compressor Attack", 0, 100, 0, 0};
//const PGM_KemperParam Param_Modulation_Rate PROGMEM =  {20, "Modulation Rate", 0, 127, 128, Options_modrate};
const PGM_KemperParam Param_Modulation_Rate PROGMEM = { 20, "Modulation Rate", 0, 100, 0, 0};
const PGM_KemperParam Param_Modulation_Depth PROGMEM =  {21, "Modulation Depth", 0, 100, 0, 0};
const PGM_KemperParam Param_Modulation_Feedback PROGMEM =  {22, "Modulation Feedback", 0, 100, 0, 0};
const PGM_KemperParam Param_Modulation_Crossover PROGMEM =  {23, "Modulation Crossover", 0, 100, 0, 0};
const PGM_KemperParam Param_Modulation_HyperChorus_Amount PROGMEM =  {24, "Modulation HyperChorus Amount", 0, 100, 0, 0};
const PGM_KemperParam Param_Modulation_Manual PROGMEM =  {25, "Modulation Manual", 0, 100, 0, 0};
const PGM_KemperParam Param_Modulation_Phaser_Peak_Spread PROGMEM =  {26, "Modulation Phaser Peak Spread", 0, 100, 0, 0};
const PGM_KemperParam Param_Modulation_Phaser_Stages PROGMEM =  {27, "Modulation Phaser Stages", 0, 5, 6, Options_modstages};
const PGM_KemperParam Param_Rotary_Speed__Slow_Off_Fast_On_ PROGMEM =  {30, "Rotary Speed (Slow-Off/Fast-On)", 0, 100, 0, 0};
const PGM_KemperParam Param_Rotary_Distance PROGMEM =  {31, "Rotary Distance", 0, 100, 0, 0};
const PGM_KemperParam Param_Rotary_Balance PROGMEM =  {32, "Rotary Balance", 0, 100, 0, 0};
const PGM_KemperParam Param_Compressor_Squash PROGMEM =  {33, "Compressor Squash", 0, 100, 0, 0};
const PGM_KemperParam Param_Graphic_EQ_Band_1 PROGMEM =  {34, "Graphic EQ Band 1", 0, 100, 0, 0};
const PGM_KemperParam Param_Graphic_EQ_Band_2 PROGMEM =  {35, "Graphic EQ Band 2", 0, 100, 0, 0};
const PGM_KemperParam Param_Graphic_EQ_Band_3 PROGMEM =  {36, "Graphic EQ Band 3", 0, 100, 0, 0};
const PGM_KemperParam Param_Graphic_EQ_Band_4 PROGMEM =  {37, "Graphic EQ Band 4", 0, 100, 0, 0};
const PGM_KemperParam Param_Graphic_EQ_Band_5 PROGMEM =  {38, "Graphic EQ Band 5", 0, 100, 0, 0};
const PGM_KemperParam Param_Graphic_EQ_Band_6 PROGMEM =  {39, "Graphic EQ Band 6", 0, 100, 0, 0};
const PGM_KemperParam Param_Graphic_EQ_Band_7 PROGMEM =  {40, "Graphic EQ Band 7", 0, 100, 0, 0};
const PGM_KemperParam Param_Graphic_EQ_Band_8 PROGMEM =  {41, "Graphic EQ Band 8", 0, 100, 0, 0};
const PGM_KemperParam Param_Parametric_EQ_Low_Gain PROGMEM =  {42, "Parametric EQ Low Gain", 0, 100, 0, 0};
const PGM_KemperParam Param_Parametric_EQ_Low_Frequency PROGMEM =  {43, "Parametric EQ Low Frequency", 0, 100, 0, 0};
const PGM_KemperParam Param_Parametric_EQ_High_Gain PROGMEM =  {44, "Parametric EQ High Gain", 0, 100, 0, 0};
const PGM_KemperParam Param_Parametric_EQ_High_Frequency PROGMEM =  {45, "Parametric EQ High Frequency", 0, 100, 0, 0};
const PGM_KemperParam Param_Parametric_EQ_Peak_Gain PROGMEM =  {46, "Parametric EQ Peak Gain", 0, 100, 0, 0};
const PGM_KemperParam Param_Parametric_EQ_Peak_Frequency PROGMEM =  {47, "Parametric EQ Peak Frequency", 0, 100, 0, 0};
const PGM_KemperParam Param_Parametric_EQ_Peak_Q_Factor PROGMEM =  {48, "Parametric EQ Peak Q-Factor", 0, 100, 0, 0};
const PGM_KemperParam Param_Parametric_EQ_Peak_Gain_2 PROGMEM =  {49, "Parametric EQ Peak Gain 2", 0, 100, 0, 0};
const PGM_KemperParam Param_Parametric_EQ_Peak_Frequency_2 PROGMEM =  {50, "Parametric EQ Peak Frequency 2", 0, 100, 0, 0};
const PGM_KemperParam Param_Parametric_EQ_Peak_Q_Factor_2 PROGMEM =  {51, "Parametric EQ Peak Q-Factor 2", 0, 100, 0, 0};
const PGM_KemperParam Param_Wah_Peak_Range PROGMEM =  {52, "Wah Peak Range", 0, 100, 0, 0};
const PGM_KemperParam Param_Ducking PROGMEM =  {53, "Ducking", 0, 100, 0, 0};
const PGM_KemperParam Param_Voice_Mix PROGMEM =  {55, "Voice Mix", 0, 100, 0, 0};
const PGM_KemperParam Param_Voice_1_Pitch PROGMEM =  {56, "Voice 1 Pitch", 28, 100, 73, Options_pitch};
const PGM_KemperParam Param_Voice_2_Pitch PROGMEM =  {57, "Voice 2 Pitch", 28, 100, 73, Options_pitch};
const PGM_KemperParam Param_Detune PROGMEM =  {58, "Detune", 0, 100, 0, 0};
const PGM_KemperParam Param_Smooth_Chords PROGMEM =  {60, "Smooth Chords", 0, 1, 2, Options_b};
const PGM_KemperParam Param_Pure_Tuning PROGMEM =  {61, "Pure Tuning", 0, 1, 2, Options_b};
const PGM_KemperParam Param_Voice_1_Interval PROGMEM =  {62, "Voice 1 Interval", 42, 95, 54, Options_voiceinterval};
const PGM_KemperParam Param_Voice_2_Interval PROGMEM =  {63, "Voice 2 Interval", 42, 95, 54, Options_voiceinterval};
const PGM_KemperParam Param_Key PROGMEM =  {64, "Key", 0, 11, 12, Options_key};
const PGM_KemperParam Param_Free_Formants PROGMEM =  {65, "Free Formants", 0, 1, 2, Options_b};
const PGM_KemperParam Param_Formant_Shift PROGMEM =  {66, "Formant Shift", 0, 100, 0, 0};

// Kemper Stomp Parameters
const PGM_KemperParam * const Params_Wah_Wah[] PROGMEM = {&Param_Mix, &Param_Volume, &Param_Wah_Manual, &Param_Wah_Peak, &Param_Wah_Range, &Param_Wah_Pedal_Mode, &Param_Wah_Peak_Range, &Param_Ducking};
const PGM_KemperParam * const Params_Wah_Low_Pass[] PROGMEM = {&Param_Mix, &Param_Volume, &Param_Wah_Manual, &Param_Wah_Peak, &Param_Wah_Range, &Param_Wah_Pedal_Mode, &Param_Wah_Peak_Range, &Param_Ducking};
const PGM_KemperParam * const Params_Wah_High_Pass[] PROGMEM = {&Param_Mix, &Param_Volume, &Param_Wah_Manual, &Param_Wah_Peak, &Param_Wah_Range, &Param_Wah_Pedal_Mode, &Param_Wah_Peak_Range, &Param_Ducking};
const PGM_KemperParam * const Params_Wah_Vowel_Filter[] PROGMEM = {&Param_Mix, &Param_Volume, &Param_Wah_Manual, &Param_Wah_Peak, &Param_Wah_Range, &Param_Wah_Pedal_Mode, &Param_Wah_Peak_Range, &Param_Ducking};
const PGM_KemperParam * const Params_Wah_Phaser[] PROGMEM = {&Param_Mix, &Param_Volume, &Param_Wah_Manual, &Param_Wah_Peak, &Param_Wah_Range, &Param_Wah_Pedal_Mode, &Param_Modulation_Phaser_Peak_Spread, &Param_Modulation_Phaser_Stages, &Param_Wah_Peak_Range, &Param_Ducking};
const PGM_KemperParam * const Params_Wah_Flanger[] PROGMEM = {&Param_Mix, &Param_Volume, &Param_Wah_Manual, &Param_Wah_Peak, &Param_Wah_Range, &Param_Wah_Pedal_Mode, &Param_Wah_Peak_Range, &Param_Ducking};
const PGM_KemperParam * const Params_Wah_Rate_Reducer[] PROGMEM = {&Param_Mix, &Param_Volume, &Param_Wah_Manual, &Param_Wah_Peak, &Param_Wah_Range, &Param_Wah_Pedal_Mode, &Param_Wah_Peak_Range, &Param_Ducking};
const PGM_KemperParam * const Params_Wah_Ring_Modulator[] PROGMEM = {&Param_Mix, &Param_Volume, &Param_Wah_Manual, &Param_Wah_Range, &Param_Wah_Pedal_Mode, &Param_Ducking};
const PGM_KemperParam * const Params_Wah_Freq_Shifter[] PROGMEM = {&Param_Mix, &Param_Volume, &Param_Wah_Manual, &Param_Wah_Range, &Param_Wah_Pedal_Mode, &Param_Ducking};
const PGM_KemperParam * const Params_Pedal_Pitch[] PROGMEM = {&Param_Mix, &Param_Volume, &Param_Ducking, &Param_Voice_1_Pitch, &Param_Voice_2_Pitch};
const PGM_KemperParam * const Params_Wah_Formant_Shift[] PROGMEM = {&Param_Mix, &Param_Volume, &Param_Wah_Manual, &Param_Wah_Range, &Param_Wah_Pedal_Mode, &Param_Ducking, &Param_Voice_2_Pitch};
const PGM_KemperParam * const Params_Pedal_Vinyl_Stop[] PROGMEM = {&Param_Mix, &Param_Volume};
const PGM_KemperParam * const Params_Bit_Shaper[] PROGMEM = {&Param_Volume, &Param_Distortion_Shaper_Drive, &Param_Distortion_Booster_Tone};
const PGM_KemperParam * const Params_Recti_Shaper[] PROGMEM = {&Param_Volume, &Param_Distortion_Shaper_Drive, &Param_Ducking};
const PGM_KemperParam * const Params_Soft_Shaper[] PROGMEM = {&Param_Volume, &Param_Distortion_Shaper_Drive};
const PGM_KemperParam * const Params_Hard_Shaper[] PROGMEM = {&Param_Volume, &Param_Distortion_Shaper_Drive};
const PGM_KemperParam * const Params_Wave_Shaper[] PROGMEM = {&Param_Volume, &Param_Distortion_Shaper_Drive};
const PGM_KemperParam * const Params_Green_Screem[] PROGMEM = {&Param_Volume, &Param_Distortion_Shaper_Drive, &Param_Distortion_Booster_Tone};
const PGM_KemperParam * const Params_Plus_DS[] PROGMEM = {&Param_Volume, &Param_Distortion_Shaper_Drive};
const PGM_KemperParam * const Params_One_DS[] PROGMEM = {&Param_Volume, &Param_Distortion_Shaper_Drive, &Param_Distortion_Booster_Tone};
const PGM_KemperParam * const Params_Muffin[] PROGMEM = {&Param_Volume, &Param_Distortion_Shaper_Drive, &Param_Distortion_Booster_Tone};
const PGM_KemperParam * const Params_Mouse[] PROGMEM = {&Param_Volume, &Param_Distortion_Shaper_Drive, &Param_Distortion_Booster_Tone};
const PGM_KemperParam * const Params_Fuzz_DS[] PROGMEM = {&Param_Volume, &Param_Distortion_Shaper_Drive};
const PGM_KemperParam * const Params_Metal_DS[] PROGMEM = {&Param_Volume, &Param_Distortion_Shaper_Drive, &Param_Parametric_EQ_Low_Gain, &Param_Parametric_EQ_High_Gain, &Param_Parametric_EQ_Peak_Gain, &Param_Parametric_EQ_Peak_Frequency};
const PGM_KemperParam * const Params_Compressor[] PROGMEM = {&Param_Mix, &Param_Volume, &Param_Compressor_Gate_Intensity, &Param_Compressor_Attack, &Param_Compressor_Squash};
const PGM_KemperParam * const Params_Gate_2_1[] PROGMEM = {&Param_Compressor_Gate_Intensity};
const PGM_KemperParam * const Params_Gate_4_1[] PROGMEM = {&Param_Compressor_Gate_Intensity};
const PGM_KemperParam * const Params_Vintage_Chorus[] PROGMEM = {&Param_Mix, &Param_Volume, &Param_Modulation_Rate, &Param_Modulation_Depth, &Param_Modulation_Crossover, &Param_Ducking};
const PGM_KemperParam * const Params_Hyper_Chorus[] PROGMEM = {&Param_Mix, &Param_Volume, &Param_Modulation_Depth, &Param_Modulation_Crossover, &Param_Modulation_HyperChorus_Amount, &Param_Ducking};
const PGM_KemperParam * const Params_Air_Chorus[] PROGMEM = {&Param_Volume, &Param_Modulation_Depth, &Param_Modulation_Crossover};
const PGM_KemperParam * const Params_Vibrato[] PROGMEM = {&Param_Volume, &Param_Modulation_Rate, &Param_Modulation_Depth, &Param_Modulation_Crossover};
const PGM_KemperParam * const Params_Rotary_Speaker[] PROGMEM = {&Param_Mix, &Param_Volume, &Param_Rotary_Speed__Slow_Off_Fast_On_, &Param_Rotary_Distance, &Param_Rotary_Balance, &Param_Ducking};
const PGM_KemperParam * const Params_Tremolo[] PROGMEM = {&Param_Volume, &Param_Modulation_Rate, &Param_Modulation_Depth, &Param_Modulation_Crossover, &Param_Ducking};
const PGM_KemperParam * const Params_Micro_Pitch[] PROGMEM = {&Param_Mix, &Param_Volume, &Param_Modulation_Depth, &Param_Modulation_Crossover, &Param_Ducking};
const PGM_KemperParam * const Params_Phaser[] PROGMEM = {&Param_Mix, &Param_Volume, &Param_Modulation_Rate, &Param_Modulation_Depth, &Param_Modulation_Feedback, &Param_Modulation_Manual, &Param_Modulation_Phaser_Peak_Spread, &Param_Modulation_Phaser_Stages, &Param_Ducking};
const PGM_KemperParam * const Params_Phase_Vibe[] PROGMEM = {&Param_Mix, &Param_Volume, &Param_Modulation_Rate, &Param_Modulation_Depth, &Param_Modulation_Feedback, &Param_Modulation_Manual, &Param_Modulation_Phaser_Peak_Spread, &Param_Modulation_Phaser_Stages, &Param_Ducking};
const PGM_KemperParam * const Params_Phaser_Oneway[] PROGMEM = {&Param_Mix, &Param_Volume, &Param_Modulation_Rate, &Param_Modulation_Depth, &Param_Modulation_Feedback, &Param_Modulation_Manual, &Param_Modulation_Phaser_Peak_Spread, &Param_Modulation_Phaser_Stages, &Param_Ducking};
const PGM_KemperParam * const Params_Flanger[] PROGMEM = {&Param_Mix, &Param_Volume, &Param_Modulation_Rate, &Param_Modulation_Depth, &Param_Modulation_Feedback, &Param_Modulation_Manual, &Param_Ducking};
const PGM_KemperParam * const Params_Flanger_Oneway[] PROGMEM = {&Param_Mix, &Param_Volume, &Param_Modulation_Rate, &Param_Modulation_Depth, &Param_Modulation_Feedback, &Param_Modulation_Manual, &Param_Ducking};
const PGM_KemperParam * const Params_Graphic_Equalizer[] PROGMEM = {&Param_Mix, &Param_Volume, &Param_Graphic_EQ_Band_1, &Param_Graphic_EQ_Band_2, &Param_Graphic_EQ_Band_3, &Param_Graphic_EQ_Band_4, &Param_Graphic_EQ_Band_5, &Param_Graphic_EQ_Band_6, &Param_Graphic_EQ_Band_7, &Param_Graphic_EQ_Band_8, &Param_Ducking};
const PGM_KemperParam * const Params_Studio_Equalizer[] PROGMEM = {&Param_Parametric_EQ_Low_Gain, &Param_Parametric_EQ_Low_Frequency, &Param_Parametric_EQ_High_Gain, &Param_Parametric_EQ_High_Frequency, &Param_Parametric_EQ_Peak_Gain, &Param_Parametric_EQ_Peak_Frequency, &Param_Parametric_EQ_Peak_Q_Factor, &Param_Parametric_EQ_Peak_Gain_2, &Param_Parametric_EQ_Peak_Frequency_2, &Param_Parametric_EQ_Peak_Q_Factor_2};
const PGM_KemperParam * const Params_Metal_Equalizer[] PROGMEM = {&Param_Mix, &Param_Volume, &Param_Parametric_EQ_Low_Gain, &Param_Parametric_EQ_High_Gain, &Param_Parametric_EQ_Peak_Gain, &Param_Parametric_EQ_Peak_Frequency, &Param_Ducking};
const PGM_KemperParam * const Params_Stereo_Widener[] PROGMEM = {&Param_Modulation_Rate, &Param_Modulation_Depth, &Param_Ducking};
const PGM_KemperParam * const Params_Treble_Booster[] PROGMEM = {&Param_Volume, &Param_Distortion_Booster_Tone, &Param_Ducking};
const PGM_KemperParam * const Params_Lead_Booster[] PROGMEM = {&Param_Volume, &Param_Distortion_Booster_Tone, &Param_Ducking};
const PGM_KemperParam * const Params_Pure_Booster[] PROGMEM = {&Param_Volume};
const PGM_KemperParam * const Params_Wah_Pedal_Booster[] PROGMEM = {&Param_Volume, &Param_Wah_Range};
const PGM_KemperParam * const Params_Loop_Mono[] PROGMEM = {&Param_Mix, &Param_Volume, &Param_Ducking};
const PGM_KemperParam * const Params_Loop_Stereo[] PROGMEM = {&Param_Mix, &Param_Volume, &Param_Ducking};
const PGM_KemperParam * const Params_Loop_Distortion[] PROGMEM = {&Param_Volume};
const PGM_KemperParam * const Params_Transpose[] PROGMEM = {&Param_Voice_1_Pitch};
const PGM_KemperParam * const Params_Chromatic_Pitch[] PROGMEM = {&Param_Volume, &Param_Ducking, &Param_Voice_Mix, &Param_Voice_1_Pitch, &Param_Voice_2_Pitch, &Param_Detune, &Param_Smooth_Chords, &Param_Pure_Tuning, &Param_Free_Formants, &Param_Formant_Shift};
const PGM_KemperParam * const Params_Harmonic_Pitch[] PROGMEM = {&Param_Volume, &Param_Ducking, &Param_Voice_Mix, &Param_Pure_Tuning, &Param_Voice_1_Interval, &Param_Voice_2_Interval, &Param_Key, &Param_Free_Formants, &Param_Formant_Shift};
const PGM_KemperParam * const Params_Analog_Octaver[] PROGMEM = {&Param_Volume, &Param_Modulation_Crossover, &Param_Ducking, &Param_Voice_Mix};

// Kemper Stomps
const PGM_StompInfo AllStomps[] PROGMEM = {
	{1, ColorOrange,"Wah Wah", 8, Params_Wah_Wah},
	{2, ColorOrange,"Wah Low Pass", 8, Params_Wah_Low_Pass},
	{3, ColorOrange,"Wah High Pass", 8, Params_Wah_High_Pass},
	{4, ColorOrange,"Wah Vowel Filter", 8, Params_Wah_Vowel_Filter},
	{6, ColorOrange,"Wah Phaser", 10, Params_Wah_Phaser},
	{7, ColorOrange,"Wah Flanger", 8, Params_Wah_Flanger},
	{8, ColorOrange,"Wah Rate Reducer", 8, Params_Wah_Rate_Reducer},
	{9, ColorOrange,"Wah Ring Modulator", 6, Params_Wah_Ring_Modulator},
	{10, ColorOrange,"Wah Freq Shifter", 6, Params_Wah_Freq_Shifter},
	{11, ColorOrange,"Pedal Pitch", 5, Params_Pedal_Pitch},
	{12, ColorOrange,"Wah Formant Shift", 7, Params_Wah_Formant_Shift},
	{13, ColorLightGreen,"Pedal Vinyl Stop", 2, Params_Pedal_Vinyl_Stop},
	{17, ColorRed,"Bit Shaper", 3, Params_Bit_Shaper},
	{18, ColorRed,"Recti Shaper", 3, Params_Recti_Shaper},
	{19, ColorRed,"Soft Shaper", 2, Params_Soft_Shaper},
	{20, ColorRed,"Hard Shaper", 2, Params_Hard_Shaper},
	{21, ColorRed,"Wave Shaper", 2, Params_Wave_Shaper},
	{33, ColorRed,"Green Screem", 3, Params_Green_Screem},
	{34, ColorRed,"Plus DS", 2, Params_Plus_DS},
	{35, ColorRed,"One DS", 3, Params_One_DS},
	{36, ColorRed,"Muffin", 3, Params_Muffin},
	{37, ColorRed,"Mouse", 3, Params_Mouse},
	{38, ColorRed,"Fuzz DS", 2, Params_Fuzz_DS},
	{39, ColorRed,"Metal DS", 6, Params_Metal_DS},
	{49, ColorCyan,"Compressor", 5, Params_Compressor},
	{57, ColorCyan,"Gate 2:1", 1, Params_Gate_2_1},
	{58, ColorCyan,"Gate 4:1", 1, Params_Gate_4_1},
	{64, ColorGreen,"Space", 0, 0},
	{65, ColorBlue,"Vintage Chorus", 6, Params_Vintage_Chorus},
	{66, ColorBlue,"Hyper Chorus", 6, Params_Hyper_Chorus},
	{67, ColorBlue,"Air Chorus", 3, Params_Air_Chorus},
	{68, ColorBlue,"Vibrato", 4, Params_Vibrato},
	{69, ColorBlue,"Rotary Speaker", 6, Params_Rotary_Speaker},
	{70, ColorBlue,"Tremolo", 5, Params_Tremolo},
	{71, ColorBlue,"Micro Pitch", 5, Params_Micro_Pitch},
	{81, ColorBlue,"Phaser", 9, Params_Phaser},
	{82, ColorBlue,"Phase Vibe", 9, Params_Phase_Vibe},
	{83, ColorBlue,"Phaser Oneway", 9, Params_Phaser_Oneway},
	{89, ColorBlue,"Flanger", 7, Params_Flanger},
	{91, ColorBlue,"Flanger Oneway", 7, Params_Flanger_Oneway},
	{97, ColorYellow,"Graphic Equalizer", 11, Params_Graphic_Equalizer},
	{98, ColorYellow,"Studio Equalizer", 10, Params_Studio_Equalizer},
	{99, ColorYellow,"Metal Equalizer", 7, Params_Metal_Equalizer},
	{101, ColorYellow,"Stereo Widener", 3, Params_Stereo_Widener},
	{113, ColorRed,"Treble Booster", 3, Params_Treble_Booster},
	{114, ColorRed,"Lead Booster", 3, Params_Lead_Booster},
	{115, ColorRed,"Pure Booster", 1, Params_Pure_Booster},
	{116, ColorRed,"Wah Pedal Booster", 2, Params_Wah_Pedal_Booster},
	{121, ColorWhite,"Loop Mono", 3, Params_Loop_Mono},
	{122, ColorWhite,"Loop Stereo", 3, Params_Loop_Stereo},
	{123, ColorWhite,"Loop Distortion", 1, Params_Loop_Distortion},
	{129, ColorLightGreen,"Transpose", 1, Params_Transpose},
	{130, ColorLightGreen,"Chromatic Pitch", 10, Params_Chromatic_Pitch},
	{131, ColorLightGreen,"Harmonic Pitch", 9, Params_Harmonic_Pitch},
	{132, ColorLightGreen,"Analog Octaver", 4, Params_Analog_Octaver},
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

#define MAX_KEMPER_PARAM_OPTION_LENGTH 128
#define MAX_KEMPER_PARAM_LENGTH 11


bool loadStompInfo(StompInfo *dst, int stompId, int id);
void loadStompParameters(PartialParameter *parameter, StompInfo *info = 0);
bool updateStompParameterValue(PartialParameter *parameter, int value);
int getOptionValue(PartialParameter *parameter, int optionIndex);

void testParam(int a, int b);


END_KEMPER_NAMESPACE