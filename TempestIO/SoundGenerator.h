
#ifndef SOUNDGENERATOR_H
#define SOUNDGENERATOR_H

#include "SoundChannel.h"

class SoundGenerator
{
public:
	void ReadWaveData(int16_t *buffer, int sampleCount);

private:
	SoundChannel	channels[8];
};


#endif
