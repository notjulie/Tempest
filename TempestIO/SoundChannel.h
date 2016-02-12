
#ifndef SOUNDCHANNEL_H
#define SOUNDCHANNEL_H

#ifdef _WIN32
   #pragma warning(push)
   #pragma warning(disable : 4820)	// padding in structures
#endif

class SoundChannelState
{
public:
   SoundChannelState(void);

   uint8_t GetFrequency(void) const { return frequency; }
   uint8_t GetVolume(void) const { return volume; }
   uint8_t GetVolumeAndWaveform(void) const;
   uint8_t GetWaveform(void) const { return waveform; }
   void SetFrequency(uint8_t frequency) { this->frequency = frequency; }
   void SetVolumeAndWaveform(uint8_t volumeAndWaveform);

   bool operator==(const SoundChannelState &state) const;
   bool operator!=(const SoundChannelState &state) const;

private:
   uint8_t frequency;
   uint8_t volume;
   uint8_t waveform;
};


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
