
#ifndef TEMPESTDISCOAUDIOIO_H
#define TEMPESTDISCOAUDIOIO_H

#include "TempestIO/AbstractTempestIO.h"


class TempestDiscoAudioIO :
	public AbstractTempestSoundOutput,
	public AbstractArcadeGameControlPanelReader
{
public:
	TempestDiscoAudioIO(void);

	virtual void SetSoundChannelState(int channel, SoundChannelState state);
	virtual void SetTime(uint64_t clockCycles);
   virtual uint8_t GetButtons(void);
   virtual uint8_t GetEncoder(void);
   virtual void SetButtonLED(ButtonFlag button, bool value);

private:
   uint64_t cpuTime;
};

extern TempestDiscoAudioIO	IO;


#endif
