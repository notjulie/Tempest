
#include "TempestCPU.Headers.h"

#include "AbstractSoundGenerator.h"

#include "TempestException.h"

#include "Pokey.h"



Pokey::Pokey(int _baseSoundChannel)
{
	// copy parameters
	baseSoundChannel = _baseSoundChannel;
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

void Pokey::WriteByte(uint16_t address, uint8_t value, uint64_t busTime)
{
	// sanity check
	if (soundOutput == NULL)
		throw TempestException("Pokey::WriteByte: tempestIO is NULL");

	switch (address)
	{
	case 0x00:
   case 0x02:
   case 0x04:
   case 0x06:
      {
         uint8_t channel = (uint8_t)(address / 2);
         soundOutput->SetCPUTime(busTime);
         soundChannelState[channel].SetChannelFrequency(64000.0F / (1 + value));
         soundOutput->SetSoundChannelState(baseSoundChannel + channel, soundChannelState[channel]);
      }
		break;

   case 0x01:
   case 0x03:
   case 0x05:
   case 0x07:
      {
         uint8_t channel = (uint8_t)(address / 2);
         soundOutput->SetCPUTime(busTime);
         soundChannelState[channel].SetVolume((uint8_t)(value & 0xF));
         switch (value >> 4)
         {
         case 0x0:
            // The doc implies that this has something to do with both the 17-bit and
            // 5-bit polynomials... note really sure what that's about.  I have been treating
            // it as 17-bit noise in Tempest and that seems to be right.
         case 0x8:
            soundChannelState[channel].SetWaveform(Noise17Bit);
            break;

         case 0x2:	// doc says these are the same?
         case 0x6:
            soundChannelState[channel].SetWaveform(Noise5BitHalfFrequency);
            break;

         case 0x4:
         case 0xC:
            soundChannelState[channel].SetWaveform(Noise4Bit);
            break;

         case 0xA:
            soundChannelState[channel].SetWaveform(SquareWave);
            break;
         }
         soundOutput->SetSoundChannelState(baseSoundChannel + channel, soundChannelState[channel]);
      }
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


