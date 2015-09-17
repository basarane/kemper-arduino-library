

#include "Stomp.h"

USING_NAMESPACE_KEMPER


bool KEMPER_NAMESPACE::loadStompInfo(StompInfo *dst, int stompId, int id)
{
	int indexOffset = stompId >= 6?200+10*(stompId-6):0;
	//return false;
	for (int i=0;i<AllStompsCount;i++) {
		int tmpId = pgm_read_word_near(&AllStomps[i].type);
		if (tmpId == id + indexOffset) {
			const PGM_StompInfo *src = &AllStomps[i];
			dst->PGM_index = i;
			dst->type = pgm_read_word_near(&src->type);
			memcpy_P(&dst->color, &src->color, sizeof(src->color));
			strcpy_P(dst->name, src->name);
			dst->paramCount = pgm_read_word_near(&src->paramCount);
			dst->isExpWah = pgm_read_byte_near(&src->isExpWah);
			dst->isExpPitch = pgm_read_byte_near(&src->isExpPitch);
			return true;
		}
	}
	return false;
}

void KEMPER_NAMESPACE::loadStompParameters(PartialParameter *parameter, StompInfo *info /* = 0 */)
{
	if (info) {
		parameter->stompInfo = info;
		parameter->stompType = info->type;
	}
	if (parameter->stompType != parameter->stompInfo->type) {
		parameter->currentOption = 0;
		parameter->currentParam = 0;
		parameter->stompType = parameter->stompInfo->type;
	}
	parameter->paramCount = 0;
	parameter->totalParamCount = parameter->stompInfo->paramCount;
	int startParamIndex = max(parameter->currentParam - (NUMBER_OF_PARAMS_IN_LIST-1)/2, 0);
	parameter->startParamIndex = startParamIndex = max(min(startParamIndex, parameter->totalParamCount-NUMBER_OF_PARAMS_IN_LIST), 0);

	PGM_KemperParam** params = (PGM_KemperParam**)pgm_read_word_near(&AllStomps[parameter->stompInfo->PGM_index].params);

	if (!params) {
		parameter->paramCount = 0;
		parameter->optionCount = 0;
		parameter->totalOptionCount = 0;
		return;
	}

	for (int j=0;j<min(min(parameter->stompInfo->paramCount-startParamIndex, NUMBER_OF_PARAMS_IN_LIST), parameter->totalParamCount-startParamIndex);j++) {
		PGM_KemperParam *param = (PGM_KemperParam*) pgm_read_word_near(&params[startParamIndex+j]);

		parameter->params[j].number = pgm_read_word_near(&param->number);
		parameter->params[j].minValue = pgm_read_word_near(&param->minValue);
		parameter->params[j].maxValue = pgm_read_word_near(&param->maxValue);
		parameter->params[j].optionCount = pgm_read_word_near(&param->optionCount);
		
		strcpy_P(parameter->params[j].name, param->name);
		parameter->paramCount = j+1;
	}
	const PGM_KemperParam *psrc = (PGM_KemperParam*) pgm_read_word_near(&params[parameter->currentParam]);
	int j = 0;
	parameter->optionCount = 0;
	parameter->totalOptionCount = parameter->params[parameter->currentParam-startParamIndex].optionCount;

	KemperParamOption** options = (KemperParamOption**)pgm_read_word_near(&psrc->options);

	if (parameter->totalOptionCount>0)
	{
		int startOptionIndex = max(parameter->currentOption - (NUMBER_OF_OPTIONS_IN_LIST-1)/2, 0);
		startOptionIndex = min(startOptionIndex, (parameter->totalOptionCount)-NUMBER_OF_OPTIONS_IN_LIST);
		parameter->startOptionIndex = startOptionIndex = max(0, startOptionIndex);
		for (j=0;j<min(min(parameter->totalOptionCount - startOptionIndex, NUMBER_OF_OPTIONS_IN_LIST), parameter->totalOptionCount-startOptionIndex);j++) {

			KemperParamOption* option = (KemperParamOption*)pgm_read_word_near(&options[j+startOptionIndex]);

			parameter->options[j].value = pgm_read_word_near(&option->value);
			strcpy_P(parameter->options[j].name, option->name);
			parameter->optionCount = j+1;
		}
	} else {
		parameter->startOptionIndex = 0;
		parameter->optionCount = 0;
	}
}

int KEMPER_NAMESPACE::getOptionValue(PartialParameter *parameter, int optionIndex) {
	if (parameter->totalOptionCount && optionIndex<parameter->totalOptionCount && optionIndex>=0) {
		PGM_KemperParam** params = (PGM_KemperParam**)pgm_read_word_near(&AllStomps[parameter->stompInfo->PGM_index].params);
		const PGM_KemperParam *psrc = (PGM_KemperParam*)pgm_read_word_near(&params[parameter->currentParam]);
		KemperParamOption** options = (KemperParamOption**)pgm_read_word_near(&psrc->options);
		if (!options)
			return 0;
		KemperParamOption* option = (KemperParamOption*)pgm_read_word_near(&options[optionIndex]);
		return (int)pgm_read_word_near(&option->value);
	}
	else {
		return 0;
	}
}

bool KEMPER_NAMESPACE::updateStompParameterValue(PartialParameter *parameter, int value) {
	if (parameter->totalOptionCount>0) {
		PGM_KemperParam** params = (PGM_KemperParam**)pgm_read_word_near(&AllStomps[parameter->stompInfo->PGM_index].params);
		PGM_KemperParam *param = (PGM_KemperParam*) pgm_read_word_near(&params[parameter->currentParam]);
		KemperParamOption** options = (KemperParamOption**)pgm_read_word_near(&param->options);
		for (int i=0;i<parameter->totalOptionCount && options;i++) {
			KemperParamOption* option = (KemperParamOption*)pgm_read_word_near(&options[i]);
			int val = pgm_read_word_near(&option->value);
			if (val == value) {
				if (parameter->currentOption != i) {
					parameter->currentOption = i;
					return true;
				}
				return false;
			}
		}
	} else {
		if (parameter->currentValue!=value) {
			parameter->currentValue = value;
			return true;
		}
	}
	return false; 
}