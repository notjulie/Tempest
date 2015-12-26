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
   encoder = 0;
   leds = 0;

   // copy parameters
	this->stream = stream;
}


void TempestIOStreamProxy::SetSoundChannelFrequency(int channel, int frequency)
{
#if 0
   // pack the op and the channel into the first byte
   stream->Write((OP_SOUND_FREQUENCY << 5) | channel);

   // waveform in the second byte
   stream->Write(frequency);
#endif
}

void TempestIOStreamProxy::SetSoundChannelVolume(int channel, int volume)
{
#if 0
   // pack the op and the channel into the first byte
   stream->Write((OP_SOUND_VOLUME << 5) | channel);

   // waveform in the second byte
   stream->Write(volume);
#endif
}

void TempestIOStreamProxy::SetSoundChannelWaveform(int channel, int waveform)
{
#if 0
   // pack the op and the channel into the first byte
   stream->Write((OP_SOUND_WAVE << 5) | channel);

   // waveform in the second byte
   stream->Write(waveform);
#endif
}

void TempestIOStreamProxy::Tick6KHz(void)
{
#if 0
	// send just the opcode with no data
	stream->Write(OP_6KHZ_TICK << 5);

   // and check the return stream
   for (;;)
   {
      int b = stream->Read();
      if (b < 0)
         break;

      // this is silly, it's just a bit of documentation
      TempestInPacket packet;
      packet.flags1 = (uint8_t)b;
      this->buttons = packet.flags1;
      if (packet.flags1 & ENCODER_DOWN)
         --encoder;
      if (packet.flags1 & ENCODER_UP)
         ++encoder;
   }
#endif
}


void TempestIOStreamProxy::SetButtonLED(ButtonFlag button, bool value)
{
   uint8_t oldLEDs = leds;

   // update our local value
   if (value)
      leds |= button;
   else
      leds &= ~button;

   // send it if it changed
   if (leds != oldLEDs)
   {
      stream->Write(OP_BUTTON_LEDS << 5);
      stream->Write(leds);
   }
}
