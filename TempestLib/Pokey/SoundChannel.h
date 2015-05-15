
#ifndef SOUNDCHANNEL_H
#define SOUNDCHANNEL_H


#pragma warning(push)
#pragma warning(disable : 4820)	// padding in structures


class SoundChannel
{
public:
	SoundChannel(void);

	void AddWaveData(int16_t *buffer, int count);
	void SetFrequency(int frequency);
	void SetVolume(int volume) { this->volume = volume; }
	void SetOutputWave(int outputWave);

private:
	void UpdateWaveform(void);

private:
	// our current parameters
	int frequency;
	int volume;
	int outputWave;

	// our requested values
	int newOutputWave;
	int newFrequency;

	// our state variables
	float pulseWidth;
	bool outputState;
	float outputCounter;
	float noiseCounter;
	float noiseCounterCountsPerNoiseSample;
	int noiseWaveformLength;
	const bool *noiseWaveform;
};


#pragma warning(pop)

#endif
