
#ifndef WIN32TEMPESTENVIRONMENT_H
#define WIN32TEMPESTENVIRONMENT_H

#include "Win32.h"
#include "TempestCPU/AbstractTempestEnvironment.h"

#pragma warning(push)
#pragma warning(disable : 4820)	// padding in structures

class Win32TempestEnvironment : public AbstractTempestEnvironment
{
public:
	Win32TempestEnvironment(void);
	virtual ~Win32TempestEnvironment(void);

   virtual void Reset(void);
	virtual void Sleep(int ms);
	virtual void SynchronizeClock(uint64_t busMSCount);
   virtual std::string GetDatabasePathName(void);

private:
	DWORD lastTimeCheck;
	uint64_t currentTime;

	uint64_t performanceCounterTicksPerMicroSecond;
};

#pragma warning(pop)

#endif
