
#ifndef ABSTRACTREALTIMECLOCK_H
#define ABSTRACTREALTIMECLOCK_H


class AbstractRealTimeClock
{
public:
	virtual void Reset(void) = 0;
	virtual void Sync(uint64_t busMSCount) = 0;
};


#endif
