

#include "stdafx.h"
#include <msclr\lock.h>

#include "AsteroidsCPU/AsteroidsRunner.h"
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
      asteroidsRunner = new AsteroidsRunner(environment);

      // hook objects together
      asteroidsRunner->SetSoundOutput(tempestSoundIO);
      asteroidsRunner->SetControlPanel(tempestSoundIO);
   }

   Asteroids::~Asteroids(void)
	{
		// delete
      delete asteroidsRunner, asteroidsRunner = nullptr;
      delete environment, environment = nullptr;
      delete tempestSoundIO, tempestSoundIO = nullptr;
	}

   uint64_t Asteroids::GetTotalClockCycles(void)
	{
      return asteroidsRunner->GetTotalClockCycles();
	}

   VectorEnumerator ^Asteroids::GetVectorEnumerator(void)
   {
      std::vector<SimpleVector> vectors;

      if (asteroidsRunner->HaveVectorData())
      {
         // get the latest vector data
         AsteroidsVectorInterpreter vectorInterpreter;
         asteroidsRunner->GetVectorData(vectorInterpreter);

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

