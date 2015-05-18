
#include "stdafx.h"

#include "../TempestIO/AbstractTempestIO.h"

#include "TempestException.h"

#include "Pokey.h"


bool Pokey::noiseWaveformsInitialized = false;
bool Pokey::poly17[128 * 1024];
bool Pokey::poly5[32];


Pokey::Pokey(int _baseSoundChannel)
{
	// copy parameters
	baseSoundChannel = _baseSoundChannel;

	// clear
	ALLPOT = 0;
	tempestIO = NULL;

	// initialize our noise buffers
	InitializeNoiseBuffers();
}

Pokey::~Pokey(void)
{
}


uint8_t Pokey::ReadByte(uint16_t address)
{
	switch (address)
	{
	case 0x8:
		return ALLPOT;

	case 0x0A:
		// random number
		return (uint8_t)rand();

	default:
		{
			char buf[200];
			sprintf_s(buf, "Invalid Pokey read: %02X", address);
			throw TempestException(buf);
		}
	}
}

void Pokey::WriteByte(uint16_t address, uint8_t value)
{
	// sanity check
	if (tempestIO == NULL)
		throw TempestException("Pokey::WriteByte: tempestIO is NULL");

	switch (address)
	{
	case 0x00:
		tempestIO->SetSoundChannelFrequency(baseSoundChannel, value);
		break;

	case 0x01:
		tempestIO->SetSoundChannelVolume(baseSoundChannel, value & 0x0F);
		tempestIO->SetSoundChannelWaveform(baseSoundChannel, value >> 4);
		break;

	case 0x02:
		tempestIO->SetSoundChannelFrequency(baseSoundChannel + 1, value);
		break;

	case 0x03:
		tempestIO->SetSoundChannelVolume(baseSoundChannel + 1, value & 0x0F);
		tempestIO->SetSoundChannelWaveform(baseSoundChannel + 1, value >> 4);
		break;

	case 0x04:
		tempestIO->SetSoundChannelFrequency(baseSoundChannel + 2, value);
		break;

	case 0x05:
		tempestIO->SetSoundChannelVolume(baseSoundChannel + 2, value & 0x0F);
		tempestIO->SetSoundChannelWaveform(baseSoundChannel + 2, value >> 4);
		break;

	case 0x06:
		tempestIO->SetSoundChannelFrequency(baseSoundChannel + 3, value);
		break;

	case 0x07:
		tempestIO->SetSoundChannelVolume(baseSoundChannel + 3, value & 0x0F);
		tempestIO->SetSoundChannelWaveform(baseSoundChannel + 3, value >> 4);
		break;

	case 0x8:
		// audio control... for the moment I only support the simplest mode of
		// operation until I'm told I need to support more
		if (value != 0)
		{
			char buf[200];
			sprintf_s(buf, "Invalid Pokey write: %02X, %02X", address, value);
			throw TempestException(buf);
		}
		break;

	case 0xB:
		// this just latches the "pot port state", which is basically just
		// a bunch of bit inputs
		ALLPOT = GetALLPOT();
		break;

	case 0xF:
		// serial port control... not supported
		break;

	default:
		{
			char buf[200];
			sprintf_s(buf, "Invalid Pokey write: %02X, %02X", address, value);
			throw TempestException(buf);
		}
	}
}


void Pokey::InitializeNoiseBuffers(void)
{
	// never mind if we've already been here
	if (noiseWaveformsInitialized)
		return;

	// make our noise waveforms
	MakeNoise(poly17, 128 * 1024);
	MakeNoise(poly5, 32);

	// note that we're initialized
	noiseWaveformsInitialized = true;
}

void Pokey::MakeNoise(bool *buffer, int count)
{
	// just for good form I make sure there's no DC
	int	onesLeft = count / 2;
	int   zerosLeft = count - onesLeft;
	while (onesLeft>0 && zerosLeft>0)
	{
		int random = rand() % (onesLeft + zerosLeft);
		if (random > onesLeft)
		{
			buffer[onesLeft + zerosLeft - 1] = false;
			--zerosLeft;
		}
		else
		{
			buffer[onesLeft + zerosLeft - 1] = true;
			--onesLeft;
		}
	}

}

