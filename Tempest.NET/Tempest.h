
#ifndef TEMPEST_H
#define TEMPEST_H

using namespace System;
using namespace System::Threading;

#include "VectorEnumerator.h"

class CPU6502;
class TempestBus;
class Win32PerformanceCounter3KHzClock;

namespace TempestDotNET {

	public ref class Tempest
	{
	public:
		Tempest();
		~Tempest();

		String ^GetMathBoxLogData(void);
		String ^GetMathBoxStatus(void);
		String ^GetProcessorStatus(void);
		VectorEnumerator ^GetVectorEnumerator(void);
		void LoadROM(array<Byte>^ rom, int address);
		void LoadMathBoxROM(array<Byte>^ rom, char slot);
		void Start(void);
		int GetTotalClockCycles(void);

	private:
		void ThreadEntry(void);

	private:
		bool   terminated;

		CPU6502		*cpu6502;
		TempestBus	*tempestBus;
		Win32PerformanceCounter3KHzClock *clock;
		VectorData	*vectorData;

		Object  ^synchronizer;
		Thread	^thread;
		String  ^processorStatus;

	};

}

#endif
