
#ifndef TEMPEST_H
#define TEMPEST_H

#include "GameContext.h"
#include "VectorEnumerator.h"
#include "VectorGameRunner.h"


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

		// simple dispatches to the VectorGameRunner
		uint8_t  GetAccumulator(void) { return gameRunner->GetAccumulator(); }
		int      GetProgramCounter(void) { return gameRunner->GetProgramCounter(); }
		uint8_t  GetXRegister(void) { return gameRunner->GetXRegister(); }
		uint8_t  GetYRegister(void) { return gameRunner->GetYRegister(); }
		void     Step(void) { gameRunner->Step(); }
		void     Resume(void) { gameRunner->Resume(); }

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
      static Tempest^ CreateNormalInstance(String^ game);

      /// <summary>
      /// Creates an instance that interacts with the keyboard and uses the internal audio,
      /// but uses a simulated serial connection between the game and the interface
      /// </summary>
      static Tempest^ CreateStreamedInstance(String^ game);

      /// <summary>
      /// Creates an instance that interacts with the actual Tempest hardware via
      /// COM port.
      /// </summary>
      static Tempest^ Tempest::CreateCOMPortInstance(String^ game, String^ portName);

      /// <summary>
      /// Creates an instance that interacts with the keyboard and uses the internal audio,
      /// but uses a serial loopback connection between the game and the interface
      /// </summary>
      static Tempest^ Tempest::CreateLoopbackInstance(String^ game, String^ port1, String^ port2);

   private:
      /// <summary>
      /// Constructor; I keep it private because the caller is supposed to instantiate
      /// the GameContext and give ownership to us, so I want to keep that logic internal
      /// to the statis constructors.
      /// </summary>
      Tempest(String^ gameName, GameContext *_gameContext);

	private:
		Win32TempestEnvironment *environment = nullptr;
      GameContext *gameContext = nullptr;
      VectorGameRunner *gameRunner = nullptr;
      VectorGame *game = nullptr;
      String^ gameName = String::Empty;
   };
}

#endif
