
#ifndef ABSTRACTIRQCLOCK_H
#define ABSTRACTIRQCLOCK_H

// The Tempest hardware uses a counter to do a divide by 12 of the 3 KHz
// clock for the IRQ clock... thus 250Hz.  The Tempest software also does
// a divide by 256 of this as its seconds counter.
#define IRQS_PER_SECOND 250

class AbstractIRQClock
{
public:
	virtual ~AbstractIRQClock(void) {}
	virtual bool GetIRQ(void) = 0;
	virtual void ClearIRQ(void) = 0;
};

#endif
