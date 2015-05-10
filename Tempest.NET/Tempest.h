
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

		VectorEnumerator ^GetVectorEnumerator(void);
		void LoadROM(array<Byte>^ rom, int address);
		void LoadMathBoxROM(array<Byte>^ rom, char slot);
		void SetOnePlayerButton(bool pressed);
		void Start(void);
		uint64_t GetTotalClockCycles(void);

		// simple dispatches to the TempestRunner
		uint64_t GetIRQCount(void) { return tempestRunner->GetIRQCount(); }
		void MoveWheel(int delta) { tempestRunner->MoveWheel(delta); }

	private:
		Win32TempestEnvironment *environment;
		TempestRunner *tempestRunner;
		Win32WaveStreamer *waveStreamer;
		VectorData *vectorData;

		Object  ^synchronizer;
	};

}

#endif
