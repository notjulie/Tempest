
#ifndef ABSTRACTIRQCLOCK_H
#define ABSTRACTIRQCLOCK_H

#define IRQS_PER_SECOND 187.5

class AbstractIRQClock
{
public:
	virtual ~AbstractIRQClock(void) {}
	virtual bool GetIRQ(void) = 0;
	virtual void ClearIRQ(void) = 0;
};

#endif
