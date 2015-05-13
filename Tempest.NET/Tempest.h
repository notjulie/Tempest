
#ifndef TEMPEST_H
#define TEMPEST_H

#include "VectorEnumerator.h"

using namespace System;
using namespace System::Threading;

class Win32RealTimeClock;
class Win32WaveStreamer;

namespace TempestDotNET {

	public ref class Tempest
	{
	public:
		Tempest();
		~Tempest();

		String ^GetMathBoxLogData(void);
		String ^GetMathBoxStatus(void);
		String ^GetProcessorStatus(void);
		String ^GetAudioStatus(void);
		bool   IsStopped(void);

		void SetBreakpoint(int address, bool set);

		VectorEnumerator ^GetVectorEnumerator(void);
		void LoadROM(array<Byte>^ rom, int address);
		void LoadMathBoxROM(array<Byte>^ rom, char slot);
		void SetOnePlayerButton(bool pressed);
		void Start(void);
		uint64_t GetTotalClockCycles(void);

		// simple dispatches to the TempestRunner
		uint64_t GetIRQCount(void) { return tempestRunner->GetIRQCount(); }
		int      GetProgramCounter(void) { return tempestRunner->GetProgramCounter(); }
		void     MoveWheel(int delta) { tempestRunner->MoveWheel(delta); }
		void     Step(void) { tempestRunner->Step(); }
		void     Resume(void) { tempestRunner->Resume(); }

	private:
		Win32TempestEnvironment *environment;
		TempestRunner *tempestRunner;
		VectorData *vectorData;

		Object  ^synchronizer;
	};

}

#endif
