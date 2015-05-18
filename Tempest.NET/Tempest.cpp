

#include "stdafx.h"
#include <msclr\lock.h>

#include "TempestCPU/6502/CPU6502.h"
#include "TempestCPU/6502/CPU6502Exception.h"
#include "TempestCPU/TempestBus.h"
#include "TempestCPU/TempestException.h"
#include "TempestCPU/TempestRunner.h"
#include "TempestCPU/Pokey/SoundChannelStatus.h"
#include "TempestCPU/Win32/Win32TempestEnvironment.h"
#include "TempestCPU/Win32/Win32WaveStreamer.h"


#include "Tempest.h"


using namespace System;

namespace TempestDotNET {
	Tempest::Tempest(void)
	{
		// clear
		synchronizer = gcnew Object();

		// create objects
		environment = new Win32TempestEnvironment();
		tempestRunner = new TempestRunner(environment);
		vectorData = new VectorData();
	}

	Tempest::~Tempest(void)
	{
		// delete
		delete vectorData, vectorData = NULL;
		delete tempestRunner, tempestRunner = NULL;
		delete environment, environment = NULL;
	}

	array<UInt16> ^Tempest::GetAudioChannelsStatus(void)
	{
		array<UInt16> ^result = gcnew array<UInt16>(24);

		std::vector<SoundChannelStatus> status;
		tempestRunner->GetAudioChannelsStatus(status);

		for (int i = 0; i < 8; ++i)
		{
			result[i * 3] = status[i].Volume;
			result[i * 3 + 1] = status[i].Waveform;
			result[i * 3 + 2] = status[i].Frequency;
		}

		return result;
	}

	String ^Tempest::GetAudioStatus(void)
	{
		return gcnew String(environment->GetAudioStatus().c_str());
	}

	uint64_t Tempest::GetTotalClockCycles(void)
	{
		return tempestRunner->GetTotalClockCycles();
	}

	VectorEnumerator ^Tempest::GetVectorEnumerator(void)
	{
		if (tempestRunner->HaveNewVectorData())
		{
			tempestRunner->PopVectorData(*vectorData);
			return gcnew VectorEnumerator(*vectorData);
		}
		else
		{
			return nullptr;
		}
	}

	bool Tempest::IsStopped(void)
	{
		return tempestRunner->IsStopped();
	}

	void Tempest::SetBreakpoint(int address, bool set)
	{
		tempestRunner->SetBreakpoint(address, set);
	}

	void Tempest::LoadROM(array<Byte>^ rom, int address)
	{
		std::vector<uint8_t> romCopy;
		for (int i = 0; i < rom->Length; ++i)
			romCopy.push_back(rom[i]);
		tempestRunner->LoadROM(&romCopy[0], (int)romCopy.size(), address);
	}

	void Tempest::LoadMathBoxROM(array<Byte>^ rom, char slot)
	{
		std::vector<uint8_t> romCopy;
		for (int i = 0; i < rom->Length; ++i)
			romCopy.push_back(rom[i]);
		tempestRunner->LoadMathBoxROM(&romCopy[0], (int)romCopy.size(), slot);
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

