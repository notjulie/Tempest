
#ifndef SOUNDCHANNEL_H
#define SOUNDCHANNEL_H

#include "SoundChannelState.h"

#ifdef _WIN32
   #pragma warning(push)
   #pragma warning(disable : 4820)	// padding in structures
#endif


class SoundChannel
{
public:
	SoundChannel(void);

	void AddWaveData(int16_t *buffer, int count);
   uint8_t GetVolume(void) const { return requestedState.GetVolume(); }
   void SetState(SoundChannelState state) { requestedState = state; }

private:
	void UpdateWaveform(void);

private:
	// our current state
   SoundChannelState currentState;

	// our requested state
   SoundChannelState requestedState;

	// our state variables
	float actualFrequency;
	float pulseWidth;
	bool outputState;
	float outputCounter;
	float noiseCounter;
	float noiseCounterCountsPerNoiseSample;
	int noiseWaveformLength;
	const bool *noiseWaveform;
};

#ifdef _WIN32
   #pragma warning(pop)
#endif

#endif
