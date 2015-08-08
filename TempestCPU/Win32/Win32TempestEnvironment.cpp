
#include "stdafx.h"
#include "Win32TempestEnvironment.h"


class Win32Thread : public AbstractThread
{
public:
   Win32Thread(Win32TempestEnvironment::ThreadEntry *entry, void *param)
   {
      this->entry = entry;
      this->param = param;
      thread = CreateThread(
         NULL,
         0,
         (LPTHREAD_START_ROUTINE)Entry,
         this,
         0,
         &threadID
         );
   }

   virtual ~Win32Thread(void)
   {
      WaitForSingleObject(thread, INFINITE);
   }

private:
   static DWORD WINAPI Entry(LPVOID pThis) {
      ((Win32Thread*)pThis)->entry(((Win32Thread*)pThis)->param);
      return 0;
   }

private:
   Win32TempestEnvironment::ThreadEntry *entry;
   void *param;
   HANDLE thread;
   DWORD threadID;
};


Win32TempestEnvironment::Win32TempestEnvironment(void)
{
	Reset();

	LARGE_INTEGER	li;
	QueryPerformanceFrequency(&li);
	performanceCounterTicksPerMicroSecond = (uint64_t)(li.QuadPart / 1000000);
}

Win32TempestEnvironment::~Win32TempestEnvironment(void)
{
}

AbstractThread *Win32TempestEnvironment::CreateThread(ThreadEntry *entry, void *param)
{
   return new Win32Thread(entry, param);
}

void Win32TempestEnvironment::Reset(void)
{
	// set our current time to zero
	lastTimeCheck = GetTickCount();
	currentTime = 0;
}


void Win32TempestEnvironment::Sleep(int ms)
{
	::Sleep((DWORD)ms);
}

void Win32TempestEnvironment::SynchronizeClock(uint64_t busMSCount)
{
	// update our time
	DWORD now = GetTickCount();
	DWORD elapsed = now - lastTimeCheck;
	lastTimeCheck = now;
	currentTime += elapsed;

	// if Tempest's CPU/bus clock has gotten ahead of us by 8ms
	// or more we need to pause so that real time can catch up to
	// the fake 6502
	if (busMSCount > currentTime + 8)
		Sleep(14);
}

