

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

#include "VectorGameManager.h"


using namespace System;

namespace TempestDotNET {
   VectorGameManager::VectorGameManager(String^ _gameName, GameContext *_gameContext)
   {
      // save/take ownership of the gameContext
      gameContext = _gameContext;

      // copy the game name
      gameName = _gameName;

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
      if (gameName->Equals("Asteroids"))
      {
         game = new AsteroidsGame(environment);
      }
      else if (gameName->Equals("Tempest"))
      {
         game = new TempestGame(environment);
      }
      else
      {
         throw gcnew Exception("");
      }

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

   VectorGameManager^ VectorGameManager::CreateNormalInstance(String^ game)
   {
      return gcnew VectorGameManager(game, new NormalGameContext());
   }

   VectorGameManager^ VectorGameManager::CreateStreamedInstance(String^ game)
   {
      return gcnew VectorGameManager(game, new SerializedGameContext());
   }

   VectorGameManager^ VectorGameManager::CreateCOMPortInstance(String^ game, String^ portName)
   {
      std::string name;
      for (int i = 0; i < portName->Length; ++i)
         name += (char)portName[i];
      return gcnew VectorGameManager(game, new COMPortGameContext(name));
   }

   VectorGameManager^ VectorGameManager::CreateLoopbackInstance(String^ game, String^ port1, String^ port2)
   {
      std::string name1;
      for (int i = 0; i < port1->Length; ++i)
         name1 += (char)port1[i];
      std::string name2;
      for (int i = 0; i < port2->Length; ++i)
         name2 += (char)port2[i];
      return gcnew VectorGameManager(game, new LoopbackGameContext(name1, name2));
   }
}

