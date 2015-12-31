
#include "stdafx.h"

#include "../TempestIO/AbstractTempestIO.h"

#include "TempestException.h"

#include "Pokey.h"



Pokey::Pokey(int _baseSoundChannel)
{
	// copy parameters
	baseSoundChannel = _baseSoundChannel;

	// clear
	ALLPOT = 0;
   SKCTLS = 0;
	tempestIO = NULL;
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
      // if the control register has us on the initialize state we return
      // all ones
      if ((SKCTLS & 3) == 0)
         return 0xFF;

      // else we oblige with a random number
      return (uint8_t)rand();

	default:
		{
			char buf[200];
			sprintf(buf, "Invalid Pokey read: %02X", address);
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
			sprintf(buf, "Invalid Pokey write: %02X, %02X", address, value);
			throw TempestException(buf);
		}
		break;

	case 0xB:
		// this just latches the "pot port state", which is basically just
		// a bunch of bit inputs
		ALLPOT = GetALLPOT();
		break;

	case 0xF:
      SKCTLS = value;
		break;

	default:
		{
			char buf[200];
			sprintf(buf, "Invalid Pokey write: %02X, %02X", address, value);
			throw TempestException(buf);
		}
	}
}


