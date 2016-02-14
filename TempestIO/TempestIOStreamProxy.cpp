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
   // copy parameters
   this->stream = stream;

   // clear
   buttons = 0;
   encoder = 0;
   cpuTime = 0;
   lastSendTime = 0;
   leds = 0;

   // start the first packet
   StartPacket();
}


void TempestIOStreamProxy::SetSoundChannelState(int channel, SoundChannelState state)
{
   channelState[channel] = state;
}

void TempestIOStreamProxy::SetTime(uint64_t clockCycles)
{
   // update our clock
   cpuTime = clockCycles;

   // send a packet if it's time
   if ((cpuTime - lastSendTime) > SoundIOPacketReader::ClockCyclesPerPacket)
   {
      lastSendTime += SoundIOPacketReader::ClockCyclesPerPacket;

      // end the packet
      stream.EndPacket();

      // start the next packet
      StartPacket();
   }

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
   if (value)
      leds |= button;
   else
      leds &= ~button;
}


void TempestIOStreamProxy::StartPacket(void)
{
   // send the start packet
   stream.StartPacket();

   // send the flags byte
   stream.Write(leds);

   // send the mask of channels that are currently on
   uint8_t channelMask = 0;
   for (int i = 0; i < 8; ++i)
      if (channelState[i].GetVolume() != 0)
         channelMask |= (1 << i);
   stream.Write(channelMask);

   // for each channel in the channel mask send its data
   for (int i = 0; i < 8; ++i)
   {
      if (channelMask & (1 << i))
      {
         stream.Write(channelState[i].GetFrequency());
         stream.Write(channelState[i].GetVolumeAndWaveform());
      }
   }
}

