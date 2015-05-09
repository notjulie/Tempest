
#ifndef WIN32REALTIMECLOCK_H
#define WIN32REALTIMECLOCK_H

#include "../AbstractRealTimeClock.h"
#include "Win32.h"

class Win32RealTimeClock : public AbstractRealTimeClock
{
public:
	Win32RealTimeClock(void);

	virtual void Reset(void);
	virtual void Sync(uint64_t busMSCount);

private:
	DWORD lastTimeCheck;
	uint64_t currentTime;
};

#endif
