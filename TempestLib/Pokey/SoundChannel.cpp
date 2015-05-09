
#include "stdafx.h"

#include "TempestException.h"

#include "SoundChannel.h"

SoundChannel::SoundChannel(void)
{
	frequency = 0;
	volume = 0;
	outputWave = 0;
}

void SoundChannel::AddWaveData(int16_t *, int)
{
	if (volume == 0)
		return;

	switch (outputWave)
	{
	case 0:
	default:
		{
			char buf[200];
			sprintf_s(buf, "SoundChannel::AddWaveData: unsupported waveform: %d", outputWave);
			throw TempestException(buf);
		}
	}
}

