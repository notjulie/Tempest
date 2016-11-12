
#ifndef TEMPEST_H
#define TEMPEST_H

#include "AsteroidsCPU/AsteroidsRunner.h"
#include "TDNIOStreamProxy.h"
#include "VectorEnumerator.h"

using namespace System;
using namespace System::Threading;

class Win32RealTimeClock;
class AbstractTempestSoundIO;
class Win32WaveStreamer;

namespace TempestDotNET {

   public ref class Asteroids
	{
	public:
      Asteroids(TDNIOStreamProxy ^soundIO);
      Asteroids(TDNWin32TempestSoundIO ^soundIO);
      ~Asteroids(void);

		String ^GetProcessorStatus(void);
      VectorEnumerator ^GetVectorEnumerator(void);
		bool   IsStopped(void);

		void SetBreakpoint(int address, bool set);

		void Start(void);
		uint64_t GetTotalClockCycles(void);

		// simple dispatches to the AsteroidsRunner
      uint8_t  GetAccumulator(void) { return asteroidsRunner->GetAccumulator(); }
      int      GetProgramCounter(void) { return asteroidsRunner->GetProgramCounter(); }
      uint8_t  GetXRegister(void) { return asteroidsRunner->GetXRegister(); }
      uint8_t  GetYRegister(void) { return asteroidsRunner->GetYRegister(); }
      void     Step(void) { asteroidsRunner->Step(); }
      void     Resume(void) { asteroidsRunner->Resume(); }

	private:
		Win32TempestEnvironment *environment;
      AbstractTempestSoundIO *tempestSoundIO;
      AsteroidsRunner *asteroidsRunner;
	};

}

#endif
