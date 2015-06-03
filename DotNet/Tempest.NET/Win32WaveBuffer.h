
#ifndef WIN32EWAVEBUFFER_H
#define WIN32EWAVEBUFFER_H

#pragma warning(push)
#pragma warning(disable : 4820)	// padding in structures


class Win32WaveBuffer {
public:
	Win32WaveBuffer(int sampleCount);

	void Prepare(HWAVEOUT waveOut);
	void Unprepare(HWAVEOUT waveOut);
	void Play(HWAVEOUT waveOut);
	void MarkDonePlaying(void) { isPlaying = false; }

	int16_t *GetBuffer(void) { return &waveData[0]; }
	int GetSampleCount(void) { return (int)waveData.size(); }
	bool IsPlaying(void) const { return isPlaying; }

private:
	WAVEHDR waveHeader;
	bool prepared;
	bool isPlaying;
	std::vector<int16_t> waveData;
};


#pragma warning(pop)

#endif
