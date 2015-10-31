// =========================================================================
//    TempestIOStreamProxy.cpp
//    Author: Randy Rasmussen
//    Copyright: none, use at your own risk and to your heart's content
//
//		This is an implementation of TempestIO that connects to the actual
//    IO device via a stream.  My plan is to use a USB-to-serial connection,
//    but for testing it's most convenient to leave this as generic as
//    possible.
// =========================================================================

#include "stdafx.h"

#include "../TempestCPU/TempestException.h"
#include "AbstractTempestStream.h"
#include "TempestStreamProtocol.h"

#include "TempestIOStreamProxy.h"


TempestIOStreamProxy::TempestIOStreamProxy(AbstractTempestStream *stream)
{
   // clear
   buttons = 0;

   // copy parameters
	this->stream = stream;
}


void TempestIOStreamProxy::SetSoundChannelFrequency(int channel, int frequency)
{
   // pack the op and the channel into the first byte
   stream->Write((OP_SOUND_FREQUENCY << 5) | channel);

   // waveform in the second byte
   stream->Write(frequency);
}

void TempestIOStreamProxy::SetSoundChannelVolume(int channel, int volume)
{
   // pack the op and the channel into the first byte
   stream->Write((OP_SOUND_VOLUME << 5) | channel);

   // waveform in the second byte
   stream->Write(volume);
}

void TempestIOStreamProxy::SetSoundChannelWaveform(int channel, int waveform)
{
   // pack the op and the channel into the first byte
   stream->Write((OP_SOUND_WAVE << 5) | channel);

   // waveform in the second byte
   stream->Write(waveform);
}

void TempestIOStreamProxy::Tick6KHz(void)
{
	// send just the opcode with no data
	stream->Write(OP_6KHZ_TICK << 5);

   // and check the return stream
   for (;;)
   {
      int b = stream->Read();
      if (b < 0)
         break;

      TempestInPacket packet;
      packet.flags1 = (uint8_t)b;
   }
}

