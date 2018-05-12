
#ifndef TEMPEST_H
#define TEMPEST_H

#include "TDNIOStreamProxy.h"
#include "VectorEnumerator.h"

using namespace System;
using namespace System::Threading;

class Win32RealTimeClock;
class AbstractTempestSoundIO;
class Win32WaveStreamer;

namespace TempestDotNET {

	public ref class Tempest
	{
	public:
      Tempest(TDNIOStreamProxy ^soundIO);
      Tempest(TDNWin32TempestSoundIO ^soundIO);
		~Tempest(void);

		String ^GetProcessorStatus(void);
      VectorEnumerator ^GetVectorEnumerator(void);
		bool   IsStopped(void);

		void SetBreakpoint(int address, bool set);

		void Start(void);
		uint64_t GetTotalClockCycles(void);

		// simple dispatches to the TempestRunner
		uint8_t  GetAccumulator(void) { return tempestRunner->GetAccumulator(); }
		int      GetProgramCounter(void) { return tempestRunner->GetProgramCounter(); }
		uint8_t  GetXRegister(void) { return tempestRunner->GetXRegister(); }
		uint8_t  GetYRegister(void) { return tempestRunner->GetYRegister(); }
      void     SetDemoMode(void) { tempestRunner->SetDemoMode(); }
		void     Step(void) { tempestRunner->Step(); }
		void     Resume(void) { tempestRunner->Resume(); }

	private:
		Win32TempestEnvironment *environment;
      AbstractTempestSoundOutput *soundOutput;
      TempestRunner *tempestRunner;
	};

}

#endif
