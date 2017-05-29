

#include "stdafx.h"
#include <msclr\lock.h>

#include "TempestCPU/6502/CPU6502.h"
#include "TempestCPU/6502/CPU6502Exception.h"
#include "TempestCPU/TempestBus.h"
#include "TempestCPU/TempestException.h"
#include "TempestCPU/TempestRunner.h"
#include "TempestCPU/Win32/Win32TempestEnvironment.h"

#include "TempestIO/Vector/SimpleVectorDataInterpreter.h"

#include "TDNWin32TempestIO.h"

#include "Tempest.h"


using namespace System;

namespace TempestDotNET {
   Tempest::Tempest(TDNIOStreamProxy ^soundIO)
   {
      // create objects
      environment = new Win32TempestEnvironment();
      tempestSoundIO = soundIO->GetIOObject();
      tempestRunner = new TempestRunner(environment);

      // hook objects together
      tempestRunner->SetTempestIO(tempestSoundIO);
   }

   Tempest::Tempest(TDNWin32TempestSoundIO ^soundIO)
   {
      // create objects
      environment = new Win32TempestEnvironment();
      tempestSoundIO = soundIO->GetSoundIOObject();
      tempestRunner = new TempestRunner(environment);

      // hook objects together
      tempestRunner->SetTempestIO(tempestSoundIO);
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

   VectorEnumerator ^Tempest::GetVectorEnumerator(void)
   {
      // get the latest vector data
      VectorData  vectorData;
      tempestRunner->GetVectorData(vectorData);

      // make a vector generator
      SimpleVectorGenerator vectorGenerator;

      // interpret it
      SimpleVectorDataInterpreter vectorInterpretor;
      vectorInterpretor.SetVectorData(vectorData);
      vectorInterpretor.Interpret(&vectorGenerator);

      // return the result
      std::vector<SimpleVector> vectors;
      vectorGenerator.GetAllVectors(vectors);
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
}

