
#ifndef TEMPESTDISCOAUDIOIO_H
#define TEMPESTDISCOAUDIOIO_H

#include "TempestIO/AbstractTempestIO.h"


class TempestDiscoAudioIO : public AbstractTempestSoundIO {
public:
	virtual void SetSoundChannelFrequency(int channel, int frequency);
	virtual void SetSoundChannelVolume(int channel, int volume);
	virtual void SetSoundChannelWaveform(int channel, int waveform);
	virtual void Tick6KHz(void);
   virtual uint8_t GetButtons(void);
   virtual uint8_t GetEncoder(void);
};

extern TempestDiscoAudioIO	IO;


#endif
