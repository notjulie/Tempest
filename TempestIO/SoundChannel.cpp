
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
   case Noise4Bit:
   case Noise5BitHalfFrequency:
   case Noise17Bit:
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

   case SquareWave:
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

   case None:
   default:
      // add nothing to the output
      break;
   }
}


void SoundChannel::UpdateWaveform(void)
{
	switch (currentState.GetWaveform())
	{
   case Noise5BitHalfFrequency:
		// 5-bit noise polynomial, half frequency
		actualFrequency = 64000.0F / (1 + currentState.GetFrequency()) / 2;
		pulseWidth = 44100.0F / actualFrequency;
		noiseCounterCountsPerNoiseSample = 1790000 / actualFrequency;
		if (outputCounter > pulseWidth)
			outputCounter = 0;
		noiseWaveformLength = 32;
		noiseWaveform = noise5;
		break;

   case Noise4Bit:
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

   case Noise17Bit:
		// 17-bit noise polynomial
      actualFrequency = 64000.0F / (1 + currentState.GetFrequency());
		pulseWidth = 44100.0F / actualFrequency;
		noiseCounterCountsPerNoiseSample = 1790000 / actualFrequency;
		if (outputCounter > pulseWidth)
			outputCounter = 0;
		noiseWaveformLength = 128 * 1024;
		noiseWaveform = noise17;
		break;

	case SquareWave:
		// pure tone
      actualFrequency = 64000.0F / (1 + currentState.GetFrequency()) / 2;
		pulseWidth = 44100.0F / actualFrequency / 2;
		outputState = true;
		outputCounter = 0;
		break;

   case None:
	default:
      outputCounter = 0;
      break;
	}
}

