
#include "stdafx.h"
#include <stdio.h>
#include <pthread.h>

#include "TempestCPU/AbstractTempestEnvironment.h"
#include "TempestCPU/TempestRunner.h"
#include "TempestIO/AbstractTempestIO.h"


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

class TempestPiIO : public AbstractTempestIO
{
public:
   TempestPiIO() { isVectorHalt = true; }

	virtual void SetSoundChannelFrequency(int channel, int frequency) {}
	virtual void SetSoundChannelVolume(int channel, int volume) {}
	virtual void SetSoundChannelWaveform(int channel, int waveform) {}
	virtual void Tick6KHz(void) {}

	virtual void WriteVectorRAM(uint16_t address, uint8_t value) {}
	virtual bool IsVectorHalt(void) { return isVectorHalt; }
	virtual void VectorGo(void) { isVectorHalt = false; }
	virtual void VectorReset(void) { isVectorHalt = true; }

private:
   bool isVectorHalt;
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
