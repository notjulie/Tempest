

#include "Tempest.NET.h"
#include <msclr\lock.h>

#include "AsteroidsCPU/AsteroidsGame.h"
#include "TempestCPU/6502/CPU6502.h"
#include "TempestCPU/6502/CPU6502Exception.h"
#include "TempestCPU/TempestBus.h"
#include "TempestCPU/TempestException.h"
#include "TempestCPU/TempestGame.h"
#include "TempestCPU/Win32/Win32TempestEnvironment.h"

#include "TempestIO/Vector/SimpleVectorDataInterpreter.h"

#include "VectorMultiGame.h"

#include "VectorGameManager.h"


using namespace System;

static VectorGame *CreateOurVectorGame(AbstractTempestEnvironment *environment);

namespace TempestDotNET {
   VectorGameManager::VectorGameManager(GameContext *_gameContext)
   {
      // save/take ownership of the gameContext
      gameContext = _gameContext;

      // create objects
      environment = new Win32TempestEnvironment();
   }

   VectorGameManager::~VectorGameManager(void)
	{
		// delete the runner first since it is a thread object
      delete gameRunner, gameRunner = nullptr;

      // then we can get rid of the rest
      delete game, game = nullptr;
      delete environment, environment = nullptr;
      delete gameContext, gameContext = nullptr;
	}
   
   VectorEnumerator ^VectorGameManager::GetVectorEnumerator(void)
   {
      std::vector<SimpleVector> vectors;
      if (game != nullptr)
         game->GetAllVectors(vectors);
      return gcnew VectorEnumerator(vectors);
   }
   
   bool VectorGameManager::IsStopped(void)
	{
		return gameRunner->IsStopped();
	}

	void VectorGameManager::SetBreakpoint(int address, bool set)
	{
		gameRunner->SetBreakpoint(address, set);
	}

	void VectorGameManager::Start(void)
	{
      // create the game instance
      game = CreateOurVectorGame(environment);

      game->SetSoundOutput(gameContext->GetSoundOutput());
      game->SetControlPanel(gameContext->GetControlPanelReader());
 
      gameRunner = new VectorGameRunner(game);
      gameRunner->Start();
	}

	String ^VectorGameManager::GetProcessorStatus(void)
	{
      if (gameRunner == nullptr)
         return gcnew String("Not running");
      else if (gameRunner->IsTerminated())
			return gcnew String(gameRunner->GetProcessorStatus().c_str());
		else
			return gcnew String("OK");
	}

   VectorGameManager^ VectorGameManager::CreateNormalInstance(void)
   {
      return gcnew VectorGameManager(new NormalGameContext());
   }

   VectorGameManager^ VectorGameManager::CreateStreamedInstance(void)
   {
      return gcnew VectorGameManager(new SerializedGameContext());
   }

   VectorGameManager^ VectorGameManager::CreateCOMPortInstance(String^ portName)
   {
      std::string name;
      for (int i = 0; i < portName->Length; ++i)
         name += (char)portName[i];
      return gcnew VectorGameManager(new COMPortGameContext(name));
   }

   VectorGameManager^ VectorGameManager::CreateLoopbackInstance(String^ port1, String^ port2)
   {
      std::string name1;
      for (int i = 0; i < port1->Length; ++i)
         name1 += (char)port1[i];
      std::string name2;
      for (int i = 0; i < port2->Length; ++i)
         name2 += (char)port2[i];
      return gcnew VectorGameManager(new LoopbackGameContext(name1, name2));
   }
}

static VectorGame *CreateOurVectorGame(AbstractTempestEnvironment *environment)
{
   // we have to do this outside of the class because of a rule about C++ lambdas
   // in managed code
   return new VectorMultiGame(
      {
         [=]() { return new TempestGame(environment); },
         [=]() { return new AsteroidsGame(environment); }
      }
   );
}
