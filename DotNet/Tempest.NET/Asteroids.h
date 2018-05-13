
#ifndef TEMPEST_H
#define TEMPEST_H

#include "AsteroidsCPU/AsteroidsRunner.h"
#include "VectorEnumerator.h"

using namespace System;
using namespace System::Threading;

class Win32RealTimeClock;
class AbstractTempestSoundOutput;
class Win32WaveStreamer;

namespace TempestDotNET {

   public ref class Asteroids
	{
	public:
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

      //  simple dispatches to the control panel
      void RotateLeftDown(bool down) { controlPanel->SetInputButtonState(ROTATE_LEFT_BUTTON, down); }
      void RotateRightDown(bool down) { controlPanel->SetInputButtonState(ROTATE_RIGHT_BUTTON, down); }
      void OnePlayer(bool down) { controlPanel->SetInputButtonState(ONE_PLAYER_BUTTON, down); }
      void TwoPlayer(bool down) { controlPanel->SetInputButtonState(TWO_PLAYER_BUTTON, down); }

	private:
		Win32TempestEnvironment *environment = nullptr;
      AbstractTempestSoundOutput *soundOutput = nullptr;
      AsteroidsRunner *asteroidsRunner = nullptr;
      SimpleArcadeGameControlPanel *controlPanel = nullptr;
	};

}

#endif
