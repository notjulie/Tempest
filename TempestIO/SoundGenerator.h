
#ifndef SOUNDGENERATOR_H
#define SOUNDGENERATOR_H

#include "SoundChannel.h"

class SoundGenerator
{
public:
	void ReadWaveData(int16_t *buffer, int sampleCount);

	void SetChannelFrequency(int channel, int frequency) { channels[channel].SetFrequency(frequency); }
	void SetChannelVolume(int channel, int volume) { channels[channel].SetVolume(volume); }
	void SetChannelWaveform(int channel, int waveform) { channels[channel].SetOutputWave(waveform); }

	bool HaveSoundOutput(void);

private:
	SoundChannel	channels[8];
};


#endif
