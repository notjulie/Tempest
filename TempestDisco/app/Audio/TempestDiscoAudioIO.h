
#ifndef TEMPESTDISCOAUDIOIO_H
#define TEMPESTDISCOAUDIOIO_H

#include "TempestIO/AbstractSoundGenerator.h"
#include "TempestIO/AbstractTempestIO.h"


class TempestDiscoAudioIO :
	public AbstractSoundGenerator,
	public AbstractArcadeGameControlPanelReader
{
public:
	TempestDiscoAudioIO(void);

	virtual void SetSoundChannelState(int channel, SoundChannelState state);
   virtual uint8_t GetButtons(void);
   virtual uint8_t GetEncoder(void);
   virtual void SetButtonLED(ButtonFlag button, bool value);

private:
	virtual void ProcessTimeLapse(uint64_t clockCycles);
};

extern TempestDiscoAudioIO	IO;


#endif
