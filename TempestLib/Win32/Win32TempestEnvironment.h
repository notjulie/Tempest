
#ifndef WIN32TEMPESTENVIRONMENT_H
#define WIN32TEMPESTENVIRONMENT_H

#include "Win32.h"
#include "../AbstractTempestEnvironment.h"

class Win32TempestEnvironment : public AbstractTempestEnvironment
{
public:
	Win32TempestEnvironment(void);

	virtual void Reset(void);
	virtual void SynchronizeClock(uint64_t busMSCount);

private:
	DWORD lastTimeCheck;
	uint64_t currentTime;
};

#endif
