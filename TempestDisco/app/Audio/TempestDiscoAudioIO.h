
#ifndef TEMPESTDISCOAUDIOIO_H
#define TEMPESTDISCOAUDIOIO_H

#include "TempestIO/AbstractTempestIO.h"


class TempestDiscoAudioIO : public AbstractTempestSoundIO {
public:
	void SetSoundChannelFrequency(int channel, int frequency);
	void SetSoundChannelVolume(int channel, int volume);
	void SetSoundChannelWaveform(int channel, int waveform);
	void Tick6KHz(void);
};

extern TempestDiscoAudioIO	IO;


#endif
