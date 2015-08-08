
#ifndef TEMPESTPIENVIRONMENT_H
#define TEMPESTPIENVIRONMENT_H

#include <time.h>
#include "TempestCPU/AbstractTempestEnvironment.h"

class RPiThread : public AbstractThread
{
public:
   RPiThread(AbstractTempestEnvironment::ThreadEntry *entry, void *param) {
      this->entry = entry;
      this->param = param;
      int result = pthread_create(
         &thread,
         NULL,
         &Entry,
         this
         );
      if (result != 0)
         throw "";
   }

   virtual ~RPiThread(void) {
      pthread_join(thread, NULL);
   }

private:
   static void *Entry(void *pThis) {
      RPiThread *thread = (RPiThread*)pThis;
      thread->entry(thread->param);
      return NULL;
   }

private:
   AbstractTempestEnvironment::ThreadEntry *entry;
   void *param;
   pthread_t thread;
};


class TempestPiEnvironment : public AbstractTempestEnvironment
{
public:
   virtual AbstractThread *CreateThread(ThreadEntry *entry, void *param);
	virtual void Reset(void);
	virtual void Sleep(int ms);
	virtual void SynchronizeClock(uint64_t busMSCount);

private:
   timespec startTime;
};


#endif
