

#include "Tempest.NET.h"
#include <msclr\lock.h>

#include "NativeVectorGameManager.h"
#include "Win32GameEnvironment.h"

#include "VectorGameManager.h"


using namespace System;


namespace TempestDotNET {
   VectorGameManager::VectorGameManager(GameContext *_gameContext)
   {
      // save/take ownership of the gameContext
      gameContext = _gameContext;

      // create objects
      environment = new Win32GameEnvironment();
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

