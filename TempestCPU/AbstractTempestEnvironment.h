
#ifndef ABSTRACTTEMPESTENVIRONMENT_H
#define ABSTRACTTEMPESTENVIRONMENT_H

class AbstractThread
{
public:
   virtual ~AbstractThread(void) {}
};

class AbstractTempestEnvironment
{
public:
   typedef void ThreadEntry(void *param);

public:
   virtual ~AbstractTempestEnvironment(void) {}

   virtual AbstractThread *CreateThread(ThreadEntry *entry, void *param) = 0;
	virtual void Reset(void) = 0;
	virtual void Sleep(int ms) = 0;
	virtual void SynchronizeClock(uint64_t busMSCount) = 0;
};

#endif
