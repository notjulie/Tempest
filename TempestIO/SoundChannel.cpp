
#include "stdafx.h"

#include "../TempestCPU/TempestException.h"
#include "Noise.h"

#include "SoundChannel.h"



SoundChannel::SoundChannel(void)
{
	// clear our state variables
	noiseCounter = 0;
	outputCounter = 0;
	UpdateWaveform();
}

void SoundChannel::AddWaveData(int16_t *buffer, int count)
{
	// if we've had changes made to out input parameters, accept them now
	// and recalculate our waveforms
	if (requestedState != currentState)
	{
      currentState = requestedState;
		UpdateWaveform();
	}

   // a zero volume makes this really easy
   if (currentState.GetVolume() == 0)
      return;

   switch (currentState.GetWaveform())
	{
	case 0x0:
	case 0x2:
   case 0x4:
	case 0x6:
	case 0x8:
   case 0xC:
		// noise
		for (int i = 0; i < count; ++i)
		{
			// output a sample according to the current state
         buffer[i] = (int16_t)(buffer[i] + (outputState ? currentState.GetVolume() : -currentState.GetVolume()));

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
         buffer[i] = (int16_t)(buffer[i] + (outputState ? currentState.GetVolume() : -currentState.GetVolume()));
			outputCounter++;
			if (outputCounter >= pulseWidth)
			{
				outputCounter -= pulseWidth;
				outputState = !outputState;
			}
		}
		break;
	}
}


void SoundChannel::UpdateWaveform(void)
{
	switch (currentState.GetWaveform())
	{
	case 0x0:
		// The doc implies that this has something to do with both the 17-bit and
		// 5-bit polynomials... note really sure what that's about.  For now I just
		// treat it as 17-bit noise
		// 17-bit noise polynomial
		actualFrequency = 64000.0F / (1 + currentState.GetFrequency());
		pulseWidth = 44100.0F / actualFrequency;
		noiseCounterCountsPerNoiseSample = 1790000 / actualFrequency;
		if (outputCounter > pulseWidth)
			outputCounter = 0;
		noiseWaveformLength = 128 * 1024;
		noiseWaveform = noise17;
		break;

	case 0x2:	// doc says these are the same?
	case 0x6:
		// 5-bit noise polynomial, half frequency
		actualFrequency = 64000.0F / (1 + currentState.GetFrequency()) / 2;
		pulseWidth = 44100.0F / actualFrequency;
		noiseCounterCountsPerNoiseSample = 1790000 / actualFrequency;
		if (outputCounter > pulseWidth)
			outputCounter = 0;
		noiseWaveformLength = 32;
		noiseWaveform = noise5;
		break;

   case 0x4:
   case 0xC:
      // these may be the same or not... the doc is strange... anyway,
      // it's a 4-bit noise polynomial
      actualFrequency = 64000.0F / (1 + currentState.GetFrequency());
      pulseWidth = 44100.0F / actualFrequency;
      noiseCounterCountsPerNoiseSample = 1790000 / actualFrequency;
      if (outputCounter > pulseWidth)
         outputCounter = 0;
      noiseWaveformLength = 16;
      noiseWaveform = noise4;
      break;

	case 0x8:
		// 17-bit noise polynomial
      actualFrequency = 64000.0F / (1 + currentState.GetFrequency());
		pulseWidth = 44100.0F / actualFrequency;
		noiseCounterCountsPerNoiseSample = 1790000 / actualFrequency;
		if (outputCounter > pulseWidth)
			outputCounter = 0;
		noiseWaveformLength = 128 * 1024;
		noiseWaveform = noise17;
		break;

	case 0xA:
		// pure tone
      actualFrequency = 64000.0F / (1 + currentState.GetFrequency()) / 2;
		pulseWidth = 44100.0F / actualFrequency / 2;
		outputState = true;
		outputCounter = 0;
		break;

	default:
      outputCounter = 0;
      break;
	}
}




SoundChannelState::SoundChannelState(void)
{
   volume = 0;
   waveform = 0;
   frequency = 0;
}

uint8_t SoundChannelState::GetVolumeAndWaveform(void) const
{
   return volume | (waveform << 4);
}


void SoundChannelState::SetVolumeAndWaveform(uint8_t volumeAndWaveform)
{
   volume = volumeAndWaveform & 0x0F;
   waveform = volumeAndWaveform >> 4;
}

bool SoundChannelState::operator==(const SoundChannelState &state) const
{
   // if they both have zero volume the are equal
   if (state.volume == 0 && this->volume == 0)
      return true;

   // else everything has to match
   return
      state.frequency == this->frequency &&
      state.volume == this->volume &&
      state.waveform == this->waveform;
}

bool SoundChannelState::operator!=(const SoundChannelState &state) const
{
   return !operator==(state);
}
