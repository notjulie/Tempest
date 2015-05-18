
#include "stdafx.h"

#include "TempestException.h"

#include "Pokey.h"
#include "SoundChannelStatus.h"

#include "SoundChannel.h"

SoundChannel::SoundChannel(void)
{
	// initialize our inputs
	frequency = 0;
	volume = 0;
	outputWave = 0;

	// and make sure we have no requested changes to those inputs
	newFrequency = 0;
	newOutputWave = 0;

	// clear our state variables
	noiseCounter = 0;
	outputCounter = 0;
	UpdateWaveform();
}

void SoundChannel::AddWaveData(int16_t *buffer, int count)
{
	// a zero volume makes this really easy
	if (volume == 0)
		return;

	// if we've had changes made to out input parameters, accept them now
	// and recalculate our waveforms
	if (newFrequency != frequency || newOutputWave != outputWave)
	{
		outputWave = newOutputWave;
		frequency = newFrequency;
		UpdateWaveform();
	}

	switch (outputWave)
	{
	case 0x0:
	case 0x2:
	case 0x6:
	case 0x8:
		// noise
		for (int i = 0; i < count; ++i)
		{
			// output a sample according to the current state
			buffer[i] = (int16_t)(buffer[i] + outputState ? volume : -volume);

			// increment our counter that tells us how long to hold the current sample
			outputCounter++;
			if (outputCounter >= pulseWidth)
			{
				outputCounter -= pulseWidth;

				// resample the noise generator
				noiseCounter += noiseCounterCountsPerNoiseSample;
				while (noiseCounter > noiseWaveformLength)
					noiseCounter -= noiseWaveformLength;
				outputState = noiseWaveform[(int)noiseCounter];
			}
		}
		break;

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
	// just note the change... the thread that's generating the output
	// can actually do the update
	this->newOutputWave = outputWave;
}

void SoundChannel::SetFrequency(int frequency) 
{
	// just note the change... the thread that's generating the output
	// can actually do the update
	this->newFrequency = frequency;
}

SoundChannelStatus SoundChannel::GetStatus(void)
{
	SoundChannelStatus result;
	result.Frequency = (uint16_t)actualFrequency;
	result.Volume = (uint8_t)volume;
	result.Waveform = (uint8_t)outputWave;
	return result;
}

void SoundChannel::UpdateWaveform(void)
{
	switch (outputWave)
	{
	case 0x0:
		// The doc implies that this has something to do with both the 17-bit and
		// 5-bit polynomials... note really sure what that's about.  For now I just
		// treat it as 17-bit noise
		// 17-bit noise polynomial
		actualFrequency = 64000.0F / (1 + frequency);
		pulseWidth = 44100.0F / actualFrequency;
		noiseCounterCountsPerNoiseSample = 1790000 / actualFrequency;
		if (outputCounter > pulseWidth)
			outputCounter = 0;
		noiseWaveformLength = 128 * 1024;
		noiseWaveform = Pokey::Get17BitNoise();
		break;

	case 0x2:	// doc says these are the same?
	case 0x6:
		// 5-bit noise polynomial, half frequency
		actualFrequency = 64000.0F / (1 + frequency) / 2;
		pulseWidth = 44100.0F / actualFrequency;
		noiseCounterCountsPerNoiseSample = 1790000 / actualFrequency;
		if (outputCounter > pulseWidth)
			outputCounter = 0;
		noiseWaveformLength = 32;
		noiseWaveform = Pokey::Get5BitNoise();
		break;

	case 0x8:
		// 17-bit noise polynomial
		actualFrequency = 64000.0F / (1 + frequency);
		pulseWidth = 44100.0F / actualFrequency;
		noiseCounterCountsPerNoiseSample = 1790000 / actualFrequency;
		if (outputCounter > pulseWidth)
			outputCounter = 0;
		noiseWaveformLength = 128 * 1024;
		noiseWaveform = Pokey::Get17BitNoise();
		break;

	case 0xA:
		// pure tone
		actualFrequency = 64000.0F / (1 + frequency) / 2;
		pulseWidth = 44100.0F / actualFrequency / 2;
		outputState = true;
		outputCounter = 0;
		break;

	default:
		break;
	}
}
