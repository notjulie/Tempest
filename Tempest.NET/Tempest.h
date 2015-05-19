
#ifndef TEMPEST_H
#define TEMPEST_H

#include "VectorEnumerator.h"

using namespace System;
using namespace System::Threading;

class Win32RealTimeClock;
class Win32TempestIO;
class Win32WaveStreamer;

namespace TempestDotNET {

	public ref class Tempest
	{
	public:
		Tempest();
		~Tempest();

		String ^GetProcessorStatus(void);
		String ^GetAudioStatus(void);
		bool   IsStopped(void);

		void SetBreakpoint(int address, bool set);

		VectorEnumerator ^GetVectorEnumerator(void);
		void LoadROM(array<Byte>^ rom, int address);
		void LoadMathBoxROM(array<Byte>^ rom, char slot);
		void Start(void);
		uint64_t GetTotalClockCycles(void);

		// simple dispatches to the TempestRunner
 		void     Fire(bool pressed) { tempestRunner->SetButtonState(FIRE_BUTTON, pressed); }
		uint8_t  GetAccumulator(void) { return tempestRunner->GetAccumulator(); }
		uint64_t GetIRQCount(void) { return tempestRunner->GetIRQCount(); }
		int      GetProgramCounter(void) { return tempestRunner->GetProgramCounter(); }
		uint8_t  GetXRegister(void) { return tempestRunner->GetXRegister(); }
		uint8_t  GetYRegister(void) { return tempestRunner->GetYRegister(); }
		void     MoveWheel(int delta) { tempestRunner->MoveWheel(delta); }
		void		SetOnePlayerButton(bool pressed) { tempestRunner->SetButtonState(ONE_PLAYER_BUTTON, pressed); }
		void     Step(void) { tempestRunner->Step(); }
		void     Resume(void) { tempestRunner->Resume(); }
		void     Zap(bool pressed) { tempestRunner->SetButtonState(ZAPPER_BUTTON, pressed); }

	private:
		Win32TempestEnvironment *environment;
		Win32TempestIO *tempestIO;
		TempestRunner *tempestRunner;
		VectorData *vectorData;

		Object  ^synchronizer;
	};

}

#endif
