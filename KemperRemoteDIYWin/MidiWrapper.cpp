#include "MidiWrapper.h"

#include "midi_Namespace.h"
#include "midi_Defs.h"

USING_NAMESPACE_MIDI

MidiInWrapper* pMidiIn;

MidiType getTypeFromStatusByte(byte inStatus)
{
    if ((inStatus  < 0x80) ||
        (inStatus == 0xf4) ||
        (inStatus == 0xf5) ||
        (inStatus == 0xf9) ||
        (inStatus == 0xfD))
    {
        // Data bytes and undefined.
        return InvalidType;
    }
    if (inStatus < 0xf0)
    {
        // Channel message, remove channel nibble.
        return MidiType(inStatus & 0xf0);
    }

    return MidiType(inStatus);
}

Channel getChannelFromStatusByte(byte inStatus)
{
    return (inStatus & 0x0f) + 1;
}


void mycallback( double deltatime, std::vector< unsigned char > *message, void */*userData*/ )
{
	pMidiIn->lastMessage = message;
  unsigned int nBytes = message->size();
  unsigned char* data = message->data();
  /*
  for ( unsigned int i=0; i<nBytes; i++ )
    std::cout << "Byte " << i << " = " << (int)message->at(i) << ", ";
  if ( nBytes > 0 )
    std::cout << "stamp = " << deltatime << std::endl;
	*/
  byte channel = data[0];
  channel = getChannelFromStatusByte(data[0]);
  byte type = getTypeFromStatusByte(data[0]);
  switch (type) {
        case ActiveSensing:         if (pMidiIn->mActiveSensingCallback != 0)		  pMidiIn->mActiveSensingCallback();   break;
        case ControlChange:         if (pMidiIn->mControlChangeCallback != 0)         pMidiIn->mControlChangeCallback(channel, data[1], data[2]);    break;
        case ProgramChange:         if (pMidiIn->mProgramChangeCallback != 0)         pMidiIn->mProgramChangeCallback(channel, data[1]);    break;
		case SystemExclusive:       if (pMidiIn->mSystemExclusiveCallback != 0)       pMidiIn->mSystemExclusiveCallback(message->data(), message->size());    break;
  }
}

MidiInWrapper::MidiInWrapper() {
    mControlChangeCallback          = 0;
    mProgramChangeCallback          = 0;
    mSystemExclusiveCallback        = 0;
    mActiveSensingCallback          = 0;
	lastMessage = 0;
}

void MidiInWrapper::begin(byte channel) {
	pMidiIn = this;
	midiIn.ignoreTypes( false, false, false );
	midiIn.setCallback( &mycallback );
} 

void MidiInWrapper::read() {
	
}

const byte* MidiInWrapper::getSysExArray() {
	if (lastMessage)
		return lastMessage->data();
	return 0;
}

unsigned MidiInWrapper::getSysExArrayLength() {
	if (lastMessage)
		return lastMessage->size();
	return 0;
}

void MidiOutWrapper::begin(byte channel) {
} 

void MidiOutWrapper::sendSysEx(unsigned inLength, const byte* inArray, bool inArrayContainsBoundaries) {
	//message.clear();
	std::vector<unsigned char> message;
	if (!inArrayContainsBoundaries)
		message.push_back(0xf0);
	for (int i=0;i<inLength;i++)
		message.push_back(inArray[i]);
	if (!inArrayContainsBoundaries)
		message.push_back(0xf7);
	midiOut.sendMessage( &message );
}

void MidiOutWrapper::send(byte inType, byte inData1, byte inData2, byte inChannel) {
	//message.clear();
	std::vector<unsigned char> message;
	message.push_back(inType);
	message.push_back(inData1);
	message.push_back(inData2);
	midiOut.sendMessage( &message );
}
