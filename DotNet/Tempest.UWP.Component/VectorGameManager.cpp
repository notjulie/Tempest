#include "pch.h"

#include "AbstractGameEnvironment.h"
#include "NativeVectorGameManager.h"

#include "VectorGameManager.h"


using namespace Platform;


namespace Tempest_UWP_Component {
   VectorGameManager::VectorGameManager(GameContext *_gameContext)
   {
      // save/take ownership of the gameContext
      gameContext = _gameContext;

      // create the environment object that allows us to pass general items
      // to the games
      environment = new AbstractGameEnvironment();

      // create our database for Tempest and register it in the environment
      /*tempestDB = new TempestDB();
      {
         // get this EXE's file name
         char exeFileName[1000] = "";
         GetModuleFileNameA(NULL, exeFileName, sizeof(exeFileName));

         // strip off the file name
         size_t len = strlen(exeFileName);
         while (len > 0 && exeFileName[len - 1] != '\\')
            exeFileName[--len] = 0;

         // return the result
         std::string result = exeFileName;
         result += "Tempest.DB";
         tempestDB->Open(result);
      }
      environment->RegisterResource(AbstractTempestDB::ResourceID(), tempestDB);*/
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
      std::vector<DisplayVector> vectors;
      if (game != nullptr)
         game->GetAllVectors(vectors);
      return ref new VectorEnumerator(vectors);
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
      try {
         // create the game instance
         game = CreateOurVectorGame(environment);

         game->SetSoundOutput(gameContext->GetSoundOutput());
         game->SetControlPanel(gameContext->GetControlPanelReader());

         gameRunner = new VectorGameRunner(game);
         gameRunner->Start();
      }
      catch (std::runtime_error &e) {
         std::string s = e.what();
         throw ref new Platform::Exception(0, ref new Platform::String(std::wstring(s.begin(), s.end()).c_str()));
      }
      catch (...) {
         std::string s = "VectorGameManager::Start: unknown exception";
         throw ref new Platform::Exception(0, ref new Platform::String(std::wstring(s.begin(), s.end()).c_str()));
      }
   }

   String ^VectorGameManager::GetProcessorStatus(void)
   {
      std::string result;
      if (gameRunner == nullptr)
         result = "Not running";
      else if (gameRunner->IsTerminated())
         result = gameRunner->GetProcessorStatus();
      else
         result = "OK";

      return ref new Platform::String(std::wstring(result.begin(), result.end()).c_str());
   }

   VectorGameManager^ VectorGameManager::CreateNormalInstance(void)
   {
      return ref new VectorGameManager(new NormalGameContext());
   }

}

