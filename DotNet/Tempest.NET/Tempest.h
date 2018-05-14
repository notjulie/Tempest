
#ifndef TEMPEST_H
#define TEMPEST_H

#include "GameContext.h"
#include "VectorEnumerator.h"


using namespace System;
using namespace System::Threading;


namespace TempestDotNET {

	public ref class Tempest sealed
	{
	public:
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
      void OnePlayer(bool down) { gameContext->GetControlPanelWriter()->SetButtonState(ONE_PLAYER_BUTTON, down); }
      void TwoPlayer(bool down) { gameContext->GetControlPanelWriter()->SetButtonState(TWO_PLAYER_BUTTON, down); }
      void Fire(bool down) { gameContext->GetControlPanelWriter()->SetButtonState(FIRE_BUTTON, down); }
      void Zap(bool down) { gameContext->GetControlPanelWriter()->SetButtonState(ZAPPER_BUTTON, down); }
      bool OnePlayerLED(void) { return gameContext->GetControlPanelWriter()->GetButtonLED(ONE_PLAYER_BUTTON); }
      bool TwoPlayerLED(void) { return gameContext->GetControlPanelWriter()->GetButtonLED(TWO_PLAYER_BUTTON); }
      void MoveWheel(int distance) { gameContext->GetControlPanelWriter()->MoveEncoder(distance); }

   public:
      /// <summary>
      /// Creates an instance that interacts with the keyboard and uses the internal audio
      /// </summary>
      static Tempest^ CreateNormalInstance(void);

      /// <summary>
      /// Creates an instance that interacts with the keyboard and uses the internal audio,
      /// but uses a simulated serial connection between the game and the interface
      /// </summary>
      static Tempest^ CreateStreamedInstance(void);

      /// <summary>
      /// Creates an instance that interacts with the actual Tempest hardware via
      /// COM port.
      /// </summary>
      static Tempest^ Tempest::CreateCOMPortInstance(String^ portName);

   private:
      /// <summary>
      /// Constructor; I keep it private because the caller is supposed to instantiate
      /// the GameContext and give ownership to us, so I want to keep that logic internal
      /// to the statis constructors.
      /// </summary>
      Tempest(GameContext *_gameContext);

	protected:
		Win32TempestEnvironment *environment = nullptr;
      GameContext *gameContext = nullptr;
      TempestRunner *tempestRunner = nullptr;
	};
}

#endif
