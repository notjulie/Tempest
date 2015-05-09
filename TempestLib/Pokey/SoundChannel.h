
#ifndef SOUNDCHANNEL_H
#define SOUNDCHANNEL_H


class SoundChannel
{
public:
	SoundChannel(void);

	void AddWaveData(int16_t *buffer, int count);
	void SetFrequency(int frequency) { this->frequency = frequency; }
	void SetVolume(int volume) { this->volume = volume; }
	void SetOutputWave(int outputWave) { this->outputWave = outputWave; }

private:
	int frequency;
	int volume;
	int outputWave;
};


#endif
