

#include "stdafx.h"
#include <msclr\lock.h>

#include "TempestCPU/6502/CPU6502.h"
#include "TempestCPU/6502/CPU6502Exception.h"
#include "TempestCPU/TempestBus.h"
#include "TempestCPU/TempestException.h"
#include "TempestCPU/TempestRunner.h"
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
      tempestRunner = new TempestRunner(environment);
      tempestRunner->SetSoundOutput(gameContext->GetSoundOutput());
      tempestRunner->SetControlPanel(gameContext->GetControlPanelReader());
   }

   Tempest::~Tempest(void)
	{
		// delete the runner first since it is a thread object
      delete tempestRunner, tempestRunner = nullptr;

      // then we can get rid of the rest
      delete environment, environment = nullptr;
      delete gameContext, gameContext = nullptr;
	}
   
   uint64_t Tempest::GetTotalClockCycles(void)
	{
		return tempestRunner->GetTotalClockCycles();
	}

   VectorEnumerator ^Tempest::GetVectorEnumerator(void)
   {
      std::vector<SimpleVector> vectors;
      tempestRunner->GetAllVectors(vectors);
      return gcnew VectorEnumerator(vectors);
   }
   
   bool Tempest::IsStopped(void)
	{
		return tempestRunner->IsStopped();
	}

	void Tempest::SetBreakpoint(int address, bool set)
	{
		tempestRunner->SetBreakpoint(address, set);
	}

	void Tempest::Start(void)
	{
		tempestRunner->Start();
	}

	String ^Tempest::GetProcessorStatus(void)
	{
		if (tempestRunner->IsTerminated())
			return gcnew String(tempestRunner->GetProcessorStatus().c_str());
		else
			return gcnew String("OK");
	}

   Tempest^ Tempest::CreateNormalInstance(void)
   {
      return gcnew Tempest(new NormalGameContext());
   }

   Tempest^ Tempest::CreateStreamedInstance(void)
   {
      return gcnew Tempest(new SerializedGameContext());
   }

   Tempest^ Tempest::CreateCOMPortInstance(String^ portName)
   {
      std::string name;
      for (int i = 0; i < portName->Length; ++i)
         name += (char)portName[i];
      return gcnew Tempest(new COMPortGameContext(name));
   }
}

