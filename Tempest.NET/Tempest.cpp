

#include "stdafx.h"
#include <msclr\lock.h>

#include "TempestCPU/6502/CPU6502.h"
#include "TempestCPU/6502/CPU6502Exception.h"
#include "TempestCPU/TempestBus.h"
#include "TempestCPU/TempestException.h"
#include "TempestCPU/TempestRunner.h"
#include "TempestCPU/Win32/Win32TempestEnvironment.h"

#include "Win32TempestIO.h"

#include "Tempest.h"


using namespace System;

namespace TempestDotNET {
	Tempest::Tempest(void)
	{
		// clear
		synchronizer = gcnew Object();

		// create objects
		environment = new Win32TempestEnvironment();
		tempestIO = new Win32TempestIO();
		tempestRunner = new TempestRunner(environment);

		// hook objects together
		tempestRunner->SetTempestIO(tempestIO);
	}

	Tempest::~Tempest(void)
	{
		// delete
		delete tempestRunner, tempestRunner = NULL;
		delete tempestIO, tempestIO = NULL;
		delete environment, environment = NULL;
	}

	uint64_t Tempest::GetTotalClockCycles(void)
	{
		return tempestRunner->GetTotalClockCycles();
	}

	VectorEnumerator ^Tempest::GetVectorEnumerator(void)
	{
		std::vector<SimpleVector> vectorList;
		tempestIO->GetVectorList(vectorList);
		return gcnew VectorEnumerator(vectorList);
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

