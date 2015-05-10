
#ifndef WIN32TEMPESTENVIRONMENT_H
#define WIN32TEMPESTENVIRONMENT_H

#include "Win32.h"
#include "../AbstractTempestEnvironment.h"

#include "Win32WaveStreamer.h"

class Win32TempestEnvironment : public AbstractTempestEnvironment
{
public:
	Win32TempestEnvironment(void);
	virtual ~Win32TempestEnvironment(void);

	virtual void Reset(void);
	virtual void SynchronizeClock(uint64_t busMSCount);
	virtual void SetSoundStream(AbstractTempestWaveStream *soundStream) { audioOut.SetSource(soundStream); }

	std::string GetAudioStatus(void) { return audioOut.GetErrorString(); }

private:
	Win32WaveStreamer audioOut;
	DWORD lastTimeCheck;
	uint64_t currentTime;
};

#endif
