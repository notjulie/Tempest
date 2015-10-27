

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
	Tempest::Tempest(TDNWin32TempestIO ^io)
	{
		// create objects
		environment = new Win32TempestEnvironment();
      tempestSoundIO = io->GetIOObject();
      tempestVectorIO = io->GetIOObject();
      tempestRunner = new TempestRunner(environment);

		// hook objects together
		tempestRunner->SetTempestIO(tempestSoundIO, tempestVectorIO);
	}

   class NullTempestVectorIO : public AbstractTempestVectorIO
   {
   public:
      virtual ~NullTempestVectorIO(void) {}

      virtual void WriteVectorRAM(uint16_t address, uint8_t value) {}
      virtual bool IsVectorHalt(void) { return false; }
      virtual void VectorGo(void) {}
      virtual void VectorReset(void) {}
   };


	Tempest::Tempest(TDNIOStreamProxy ^io)
	{
		// create objects
		environment = new Win32TempestEnvironment();
      tempestSoundIO = io->GetIOObject();
      tempestVectorIO = new NullTempestVectorIO();
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

