
#ifndef WIN32WAVESTREAMER_H
#define WIN32WAVESTREAMER_H

#include "TempestIO/SoundGenerator.h"

#include "Win32WaveBuffer.h"


#pragma warning(push)
#pragma warning(disable : 4820)	// padding in structures

#define WAVE_STREAM_EVENT_QUEUE_SIZE 1000
#define WAVE_STREAM_BUFFER_SAMPLE_COUNT 2000
#define WAVE_STREAM_INPUT_BUFFER_SAMPLE_COUNT (WAVE_STREAM_BUFFER_SAMPLE_COUNT*2)

enum WaveStreamEventType {
	WAVE_EVENT_VOLUME,
	WAVE_EVENT_FREQUENCY,
	WAVE_EVENT_WAVEFORM,
	WAVE_EVENT_TICK
};

struct WaveStreamEvent {
	WaveStreamEventType	eventType;
	uint8_t	channel;
	uint8_t	value;
};

class Win32WaveStreamer
{
public:
	Win32WaveStreamer(void);
	~Win32WaveStreamer(void);

	std::string GetErrorString(void) const;

	void SetChannelFrequency(int channel, int frequency);
	void SetChannelVolume(int channel, int volume);
	void SetChannelWaveform(int channel, int waveform);
	void Tick6KHz(void);

private:
	void	CallbackThread(void);
	void  QueueEvent(const WaveStreamEvent &event);
	bool	ProcessEvent(void);
	void  ProcessTick(void);

private:
	static LONG __stdcall CallbackThreadEntry(LPVOID pThis) { ((Win32WaveStreamer *)pThis)->CallbackThread(); return 0; }

private:
	HANDLE callbackThread;
	DWORD callbackThreadID;
	HWAVEOUT waveOut;
	HANDLE queueEvent;

	bool terminating;
	bool errorReported;
	std::string errorString;
	float	sampleCounter;

	int16_t inputBuffer[WAVE_STREAM_INPUT_BUFFER_SAMPLE_COUNT];
	int samplesInInputBuffer;

	WaveStreamEvent	eventQueue[WAVE_STREAM_EVENT_QUEUE_SIZE];
	int	queueIn;
	int	queueOut;

	SoundGenerator	soundGenerator;

	Win32WaveBuffer buffer1;
	Win32WaveBuffer buffer2;
};


#pragma warning(pop)

#endif
