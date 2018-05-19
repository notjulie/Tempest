

#include "stdafx.h"
#include <msclr\lock.h>

#include "AsteroidsCPU/AsteroidsGame.h"
#include "AsteroidsCPU/AsteroidsVectorInterpreter.h"

#include "TempestCPU/6502/CPU6502.h"
#include "TempestCPU/6502/CPU6502Exception.h"
#include "TempestCPU/TempestBus.h"
#include "TempestCPU/TempestException.h"
#include "TempestCPU/Win32/Win32TempestEnvironment.h"

#include "TempestIO/Vector/SimpleVectorDataInterpreter.h"

#include "Asteroids.h"


using namespace System;

namespace TempestDotNET {
   Asteroids::Asteroids(void)
   {
      // create objects
      environment = new Win32TempestEnvironment();
      tempestSoundIO = new Win32TempestSoundIO();
      game = new AsteroidsGame(environment);
      game->SetSoundOutput(tempestSoundIO);
      game->SetControlPanel(tempestSoundIO);

      asteroidsRunner = new VectorGameRunner(game);
   }

   Asteroids::~Asteroids(void)
	{
		// delete
      delete asteroidsRunner, asteroidsRunner = nullptr;
      delete game, game = nullptr;
      delete environment, environment = nullptr;
      delete tempestSoundIO, tempestSoundIO = nullptr;
	}

   uint64_t Asteroids::GetTotalClockCycles(void)
	{
      return game->GetTotalClockCycles();
	}

   VectorEnumerator ^Asteroids::GetVectorEnumerator(void)
   {
      std::vector<SimpleVector> vectors;

      if (game->HaveVectorData())
      {
         // get the latest vector data
         AsteroidsVectorInterpreter vectorInterpreter;
         game->GetVectorData(vectorInterpreter);

         // interpret it
         vectorInterpreter.Interpret();
         vectorInterpreter.GetAllVectors(vectors);
      }

      // return the result
      return gcnew VectorEnumerator(vectors);
   }
   
   bool Asteroids::IsStopped(void)
	{
      return asteroidsRunner->IsStopped();
	}

   void Asteroids::SetBreakpoint(int address, bool set)
	{
      asteroidsRunner->SetBreakpoint(address, set);
	}

   void Asteroids::Start(void)
	{
      asteroidsRunner->Start();
	}

   String ^Asteroids::GetProcessorStatus(void)
	{
      if (asteroidsRunner->IsTerminated())
         return gcnew String(asteroidsRunner->GetProcessorStatus().c_str());
		else
			return gcnew String("OK");
	}
}

