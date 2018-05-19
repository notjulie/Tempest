

#include "stdafx.h"
#include <msclr\lock.h>

#include "TempestCPU/6502/CPU6502.h"
#include "TempestCPU/6502/CPU6502Exception.h"
#include "TempestCPU/TempestBus.h"
#include "TempestCPU/TempestException.h"
#include "TempestCPU/TempestGame.h"
#include "TempestCPU/Win32/Win32TempestEnvironment.h"

#include "TempestIO/Vector/SimpleVectorDataInterpreter.h"

#include "Tempest.h"


using namespace System;

namespace TempestDotNET {
   Tempest::Tempest(GameContext *_gameContext)
   {
      // save/take ownership of the gameContext
      gameContext = _gameContext;

      // create objects
      environment = new Win32TempestEnvironment();
      game = new TempestGame(environment);
      game->SetSoundOutput(gameContext->GetSoundOutput());
      game->SetControlPanel(gameContext->GetControlPanelReader());
      gameRunner = new VectorGameRunner(game);
   }

   Tempest::~Tempest(void)
	{
		// delete the runner first since it is a thread object
      delete gameRunner, gameRunner = nullptr;

      // then we can get rid of the rest
      delete game, game = nullptr;
      delete environment, environment = nullptr;
      delete gameContext, gameContext = nullptr;
	}
   
   VectorEnumerator ^Tempest::GetVectorEnumerator(void)
   {
      std::vector<SimpleVector> vectors;
      game->GetAllVectors(vectors);
      return gcnew VectorEnumerator(vectors);
   }
   
   bool Tempest::IsStopped(void)
	{
		return gameRunner->IsStopped();
	}

	void Tempest::SetBreakpoint(int address, bool set)
	{
		gameRunner->SetBreakpoint(address, set);
	}

	void Tempest::Start(void)
	{
		gameRunner->Start();
	}

	String ^Tempest::GetProcessorStatus(void)
	{
		if (gameRunner->IsTerminated())
			return gcnew String(gameRunner->GetProcessorStatus().c_str());
		else
			return gcnew String("OK");
	}

   Tempest^ Tempest::CreateNormalInstance(String^ game)
   {
      return gcnew Tempest(new NormalGameContext());
   }

   Tempest^ Tempest::CreateStreamedInstance(String^ game)
   {
      return gcnew Tempest(new SerializedGameContext());
   }

   Tempest^ Tempest::CreateCOMPortInstance(String^ game, String^ portName)
   {
      std::string name;
      for (int i = 0; i < portName->Length; ++i)
         name += (char)portName[i];
      return gcnew Tempest(new COMPortGameContext(name));
   }

   Tempest^ Tempest::CreateLoopbackInstance(String^ game, String^ port1, String^ port2)
   {
      std::string name1;
      for (int i = 0; i < port1->Length; ++i)
         name1 += (char)port1[i];
      std::string name2;
      for (int i = 0; i < port2->Length; ++i)
         name2 += (char)port2[i];
      return gcnew Tempest(new LoopbackGameContext(name1, name2));
   }
}

