
#include "stdafx.h"
#include "SoundGenerator.h"

void SoundGenerator::ReadWaveData(int16_t *buffer, int sampleCount)
{
	// clear the buffer
	for (int i = 0; i < sampleCount; ++i)
		buffer[i] =  0;

	// let the sound channels add their thing to it
	for (int i = 0; i < 8; ++i)
		channels[i].AddWaveData(buffer, sampleCount);
}


bool SoundGenerator::HaveSoundOutput(void)
{
	for (int i = 0; i < 8; ++i)
		if (channels[i].GetVolume() > 0)
			return true;
	return false;
}
