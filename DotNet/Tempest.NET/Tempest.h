
#ifndef TEMPEST_H
#define TEMPEST_H

#include "TDNIOStreamProxy.h"
#include "VectorEnumerator.h"
#include "Win32TempestIO.h"

using namespace System;
using namespace System::Threading;

class AbstractTempestSoundIO;

namespace TempestDotNET {

	public ref class Tempest
	{
	public:
      Tempest(TDNIOStreamProxy ^soundIO);
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

      //  simple dispatches to the control panel
      void OnePlayer(bool down) { controlPanel->SetButtonState(ONE_PLAYER_BUTTON, down); }
      void TwoPlayer(bool down) { controlPanel->SetButtonState(TWO_PLAYER_BUTTON, down); }
      void Fire(bool down) { controlPanel->SetButtonState(FIRE_BUTTON, down); }
      void Zap(bool down) { controlPanel->SetButtonState(ZAPPER_BUTTON, down); }
      bool OnePlayerLED(void) { return controlPanel->GetButtonLED(ONE_PLAYER_BUTTON); }
      bool TwoPlayerLED(void) { return controlPanel->GetButtonLED(TWO_PLAYER_BUTTON); }
      void MoveWheel(int distance) { controlPanel->MoveEncoder(distance); }

   protected:
      Tempest(void);

	protected:
		Win32TempestEnvironment *environment;
      AbstractTempestSoundOutput *soundOutput;
      AbstractArcadeGameControlPanelWriter *controlPanel = nullptr;
      TempestRunner *tempestRunner;
	};

   public ref class TempestWithInternalAudio : public Tempest
   {
   public:
      TempestWithInternalAudio(void);
      ~TempestWithInternalAudio(void);

   private:
      Win32TempestSoundIO * tempestSoundIO = nullptr;
   };


}

#endif
