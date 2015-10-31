

#include "stdafx.h"
#include <msclr\lock.h>

#include "TempestCPU/6502/CPU6502.h"
#include "TempestCPU/6502/CPU6502Exception.h"
#include "TempestCPU/TempestBus.h"
#include "TempestCPU/TempestException.h"
#include "TempestCPU/TempestRunner.h"
#include "TempestCPU/Win32/Win32TempestEnvironment.h"

#include "TDNWin32TempestIO.h"

#include "Tempest.h"


using namespace System;

namespace TempestDotNET {
   Tempest::Tempest(TDNIOStreamProxy ^soundIO, TDNWin32TempestVectorIO ^vectorIO)
   {
      // create objects
      environment = new Win32TempestEnvironment();
      tempestSoundIO = soundIO->GetIOObject();
      tempestVectorIO = vectorIO->GetVectorIOObject();
      tempestRunner = new TempestRunner(environment);

      // hook objects together
      tempestRunner->SetTempestIO(tempestSoundIO, tempestVectorIO);
   }

   Tempest::Tempest(TDNWin32TempestSoundIO ^soundIO, TDNWin32TempestVectorIO ^vectorIO)
   {
      // create objects
      environment = new Win32TempestEnvironment();
      tempestSoundIO = soundIO->GetSoundIOObject();
      tempestVectorIO = vectorIO->GetVectorIOObject();
      tempestRunner = new TempestRunner(environment);

      // hook objects together
      tempestRunner->SetTempestIO(tempestSoundIO, tempestVectorIO);
   }

   Tempest::~Tempest(void)
	{
		// delete
		delete tempestRunner, tempestRunner = NULL;
      delete environment, environment = NULL;
	}

	uint64_t Tempest::GetTotalClockCycles(void)
	{
		return tempestRunner->GetTotalClockCycles();
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
}

