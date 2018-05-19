
#ifndef TEMPEST_H
#define TEMPEST_H

#include "AsteroidsCPU/AsteroidsGame.h"
#include "VectorEnumerator.h"
#include "VectorGameRunner.h"

using namespace System;
using namespace System::Threading;

#include "Win32TempestIO.h"


namespace TempestDotNET {

   public ref class Asteroids
	{
	public:
      Asteroids(void);
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
      void RotateLeftDown(bool down) { tempestSoundIO->SetButtonState(ROTATE_LEFT_BUTTON, down); }
      void RotateRightDown(bool down) { tempestSoundIO->SetButtonState(ROTATE_RIGHT_BUTTON, down); }
      void OnePlayer(bool down) { tempestSoundIO->SetButtonState(ONE_PLAYER_BUTTON, down); }
      void TwoPlayer(bool down) { tempestSoundIO->SetButtonState(TWO_PLAYER_BUTTON, down); }
      bool OnePlayerLED(void) { return tempestSoundIO->GetButtonLED(ONE_PLAYER_BUTTON); }
      bool TwoPlayerLED(void) { return tempestSoundIO->GetButtonLED(TWO_PLAYER_BUTTON); }

	private:
		Win32TempestEnvironment *environment = nullptr;
      Win32TempestSoundIO *tempestSoundIO = nullptr;
      VectorGameRunner *asteroidsRunner = nullptr;
      AsteroidsGame *game = nullptr;
   };

}

#endif
