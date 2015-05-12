
#ifndef ABSTRACTTEMPESTENVIRONMENT_H
#define ABSTRACTTEMPESTENVIRONMENT_H

class AbstractTempestWaveStream;

class AbstractTempestEnvironment
{
public:
	virtual void Reset(void) = 0;
	virtual void Sleep(int ms) = 0;
	virtual void SynchronizeClock(uint64_t busMSCount) = 0;
	virtual void SetSoundStream(AbstractTempestWaveStream *soundStream) = 0;
};

#endif
