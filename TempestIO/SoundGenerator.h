
#ifndef SOUNDGENERATOR_H
#define SOUNDGENERATOR_H

#include "SoundChannel.h"

class SoundGenerator
{
public:
	void ReadWaveData(int16_t *buffer, int sampleCount);

	void SetChannelState(int channel, SoundChannelState state) { channels[channel].SetState(state); }

	bool HaveSoundOutput(void);

private:
	SoundChannel	channels[8];
};


#endif
