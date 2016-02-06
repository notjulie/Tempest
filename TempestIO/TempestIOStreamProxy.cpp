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


TempestIOStreamProxy::TempestIOStreamProxy(AbstractTempestStream *_stream)
   :
      stream(_stream)
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
   // pack the op and the channel into the first byte
   stream.Write((OP_SOUND_FREQUENCY << 5) | channel);

   // waveform in the second byte
   stream.Write(frequency);
}

void TempestIOStreamProxy::SetSoundChannelVolume(int channel, int volume)
{
   // pack the op and the channel into the first byte
   stream.Write((OP_SOUND_VOLUME << 5) | channel);

   // waveform in the second byte
   stream.Write(volume);
}

void TempestIOStreamProxy::SetSoundChannelWaveform(int channel, int waveform)
{
   // pack the op and the channel into the first byte
   stream.Write((OP_SOUND_WAVE << 5) | channel);

   // waveform in the second byte
   stream.Write(waveform);
}

void TempestIOStreamProxy::Tick6KHz(void)
{
   // end the current packet and start a new one
   stream.EndPacket();
   stream.StartPacket();

	// send just the opcode with no data
	stream.Write(OP_6KHZ_TICK << 5);

   // and check the return stream
   for (;;)
   {
      int b = stream.Read();
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
      stream.Write(OP_BUTTON_LEDS << 5);
      stream.Write(leds);
   }
}
