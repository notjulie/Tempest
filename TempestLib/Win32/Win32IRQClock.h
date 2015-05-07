
#ifndef WIN32IRQCLOCK_H
#define WIN32IRQCLOCK_H

#include "Win32.h"
#include "../AbstractIRQClock.h"

#pragma warning(push)
#pragma warning(disable : 4820)	// padding in structures

class Win32IRQClock : public AbstractIRQClock
{
public:
	Win32IRQClock(void);
	virtual ~Win32IRQClock(void);

	virtual bool GetIRQ(void);
	virtual void ClearIRQ(void);

private:
	void	ThreadRun(void);

private:
	static DWORD __stdcall ThreadStart(LPVOID pThis) { ((Win32IRQClock *)pThis)->ThreadRun(); return 0; }

private:
	HANDLE	thread;
	DWORD		threadID;
	uint64_t	irqsGenerated;
	uint64_t	irqsHandled;
	double	msAccumulator;
	DWORD		lastTick;
	bool		terminated;
};

#pragma warning(pop)

#endif
