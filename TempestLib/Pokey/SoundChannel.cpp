
#include "stdafx.h"

#include "TempestException.h"

#include "SoundChannel.h"

SoundChannel::SoundChannel(void)
{
	frequency = 0;
	volume = 0;
	outputWave = 0;
}

void SoundChannel::AddWaveData(int16_t *buffer, int count)
{
	if (volume < 16)
		return;

	switch (outputWave)
	{
	case 0xA:
		// pure tone
		for (int i = 0; i < count; ++i)
		{
			buffer[i] = (int16_t)(buffer[i] + outputState ? volume : -volume);
			outputCounter++;
			if (outputCounter >= pulseWidth)
			{
				outputCounter -= pulseWidth;
				outputState = !outputState;
			}
		}
		break;

	default:
		{
			char buf[200];
			sprintf_s(buf, "SoundChannel::AddWaveData: unsupported waveform: %d", outputWave);
			throw TempestException(buf);
		}
	}
}


void SoundChannel::SetOutputWave(int outputWave)
{
	if (outputWave != this->outputWave)
	{
		this->outputWave = outputWave;
		UpdateWaveform();
	}
}

void SoundChannel::SetFrequency(int frequency) 
{
	if (frequency != this->frequency)
	{
		this->frequency = frequency;
		UpdateWaveform();
	}
}


void SoundChannel::UpdateWaveform(void)
{
	switch (outputWave)
	{
	case 0xA:
		// pure tone
		{
			float f = 64000.0F / (1 + frequency);
			pulseWidth = 44100.0F / f / 2;
			outputState = true;
			outputCounter = 0;
		}
		break;

	default:
		break;
	}
}
