// ====================================================================
// Vector game emulation project
//    Author: Randy Rasmussen
//    Copyright: none... do what you will
//    Warranties: none... do what you will at your own risk
//
// File summary:
//    This is a managed C++ wrapper around the native vector game
//    implementation.
// ====================================================================


#ifndef VECTORGAMEMANAGER_H
#define VECTORGAMEMANAGER_H

#include "GameContext.h"
#include "TempestDB.h"
#include "VectorEnumerator.h"
#include "VectorGameRunner.h"


using namespace System;
using namespace System::Threading;


namespace TempestDotNET {

   /// <summary>
   /// This class is a managed wrapper around the native vector game implementation so that I
   /// can use it handily from C#.
   /// </summary>
   public ref class VectorGameManager sealed
	{
	public:
      !VectorGameManager(void);
      ~VectorGameManager(void);

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
      void MenuButton(bool down) { gameContext->GetControlPanelWriter()->SetButtonState(MENU_BUTTON, down); }
      bool OnePlayerLED(void) { return gameContext->GetControlPanelWriter()->GetButtonLED(ONE_PLAYER_BUTTON); }
      bool TwoPlayerLED(void) { return gameContext->GetControlPanelWriter()->GetButtonLED(TWO_PLAYER_BUTTON); }
      void MoveWheel(int distance) { gameContext->GetControlPanelWriter()->MoveEncoder(distance); }

   public:
      /// <summary>
      /// Creates an instance that interacts with the keyboard and uses the internal audio
      /// </summary>
      static VectorGameManager^ CreateNormalInstance(void);

      /// <summary>
      /// Creates an instance that interacts with the keyboard and uses the internal audio,
      /// but uses a simulated serial connection between the game and the interface
      /// </summary>
      static VectorGameManager^ CreateStreamedInstance(void);

      /// <summary>
      /// Creates an instance that interacts with the actual Tempest hardware via
      /// COM port.
      /// </summary>
      static VectorGameManager^ CreateCOMPortInstance(String^ portName);

      /// <summary>
      /// Creates an instance that interacts with the keyboard and uses the internal audio,
      /// but uses a serial loopback connection between the game and the interface
      /// </summary>
      static VectorGameManager^ CreateLoopbackInstance(String^ port1, String^ port2);

   private:
      /// <summary>
      /// Constructor; I keep it private because the caller is supposed to instantiate
      /// the GameContext and give ownership to us, so I want to keep that logic internal
      /// to the static constructors.
      /// </summary>
      VectorGameManager(GameContext *_gameContext);

	private:
		AbstractGameEnvironment *environment = nullptr;
      GameContext *gameContext = nullptr;
      VectorGameRunner *gameRunner = nullptr;
      VectorGame *game = nullptr;
      TempestDB *tempestDB = nullptr;
   };
}

#endif
