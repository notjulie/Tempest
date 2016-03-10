
#ifndef TEMPESTPIENVIRONMENT_H
#define TEMPESTPIENVIRONMENT_H

#include <time.h>
#include "TempestCPU/AbstractTempestEnvironment.h"

class TempestPiEnvironment : public AbstractTempestEnvironment
{
public:
   TempestPiEnvironment(void);

	virtual void Reset(void);
	virtual void Sleep(int ms);
	virtual void SynchronizeClock(uint64_t busMSCount);

private:
   timespec startTime;
   uint64_t realTimeMS;
};


#endif
