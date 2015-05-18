
#ifndef WIN32TEMPESTENVIRONMENT_H
#define WIN32TEMPESTENVIRONMENT_H

#include "Win32.h"
#include "../AbstractTempestEnvironment.h"

#include "Win32WaveStreamer.h"

#pragma warning(push)
#pragma warning(disable : 4820)	// padding in structures

class Win32TempestEnvironment : public AbstractTempestEnvironment
{
public:
	Win32TempestEnvironment(void);
	virtual ~Win32TempestEnvironment(void);

	virtual uint32_t GetMicrosecondCount(void);
	virtual void Reset(void);
	virtual void Sleep(int ms);
	virtual void SynchronizeClock(uint64_t busMSCount);
	virtual void SetSoundStream(AbstractTempestWaveStream *soundStream) { audioOut.SetSource(soundStream); }

	std::string GetAudioStatus(void) { return audioOut.GetErrorString(); }

private:
	Win32WaveStreamer audioOut;
	DWORD lastTimeCheck;
	uint64_t currentTime;
	uint64_t lastAudioUpdateBusTime;

	uint64_t performanceCounterTicksPerMicroSecond;
};

#pragma warning(pop)

#endif
