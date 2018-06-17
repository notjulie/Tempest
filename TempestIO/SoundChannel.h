// ====================================================================
// Simple sound generator
//    Author: Randy Rasmussen
//    Copyright: none... do what you will
//    Warranties: none... do what you will at your own risk
//
// File summary:
//    This plays part of a simple sound generator, whose genesis was
//    the emulation of the sound generation of the Atari Pokey chip
//    used in Tempest.  This class takes simple wave commands and
//    transforms them into an output stream of sampled audio.
//
// NOTE:
//    This class is used in an embedded controller, so no C++11
//    syntax, please.
// ====================================================================


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
