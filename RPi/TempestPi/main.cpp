
#include "stdafx.h"
#include <stdio.h>
#include <pthread.h>

#include "TempestCPU/AbstractTempestEnvironment.h"
#include "TempestCPU/TempestRunner.h"

#include "TempestPiIO.h"


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
   virtual AbstractThread *CreateThread(ThreadEntry *entry, void *param)
   {
      return new RPiThread(entry, param);
   }
	virtual void Reset(void) {}
	virtual void Sleep(int ms) {}
	virtual void SynchronizeClock(uint64_t busMSCount) {}
};

int main()
{
    TempestPiEnvironment environment;
    TempestPiIO io;

    TempestRunner tempestRunner(&environment);
    tempestRunner.SetTempestIO(&io);

    tempestRunner.Start();

    sleep(10);
    uint64_t clockCycles = tempestRunner.GetTotalClockCycles();
    printf("Total clock cycles: %ld\n", (int)clockCycles);
}
