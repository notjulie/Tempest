
#ifndef TEMPEST_H
#define TEMPEST_H

using namespace System;
using namespace System::Threading;

class CPU6502;
class TempestBus;
class Win32PerformanceCounter3KHzClock;

namespace TempestDotNET {

	public ref class Tempest
	{
	public:
		Tempest();
		~Tempest();

		String ^GetMathBoxStatus(void);
		String ^GetProcessorStatus(void);
		void LoadROM(array<Byte>^ rom, int address);
		void Start(void);

	private:
		void ThreadEntry(void);

	private:
		bool   terminated;

		CPU6502		*cpu6502;
		TempestBus	*tempestBus;
		Win32PerformanceCounter3KHzClock *clock;

		Thread	^thread;
		String  ^processorStatus;
	};

}

#endif
