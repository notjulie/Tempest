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
}


void TempestIOStreamProxy::SetSoundChannelFrequency(int channel, int frequency)
{
   currentState.SetSoundChannelFrequency(channel, frequency);
}

void TempestIOStreamProxy::SetSoundChannelVolume(int channel, int volume)
{
   currentState.SetSoundChannelVolume(channel, volume);
}

void TempestIOStreamProxy::SetSoundChannelWaveform(int channel, int waveform)
{
   currentState.SetSoundChannelWaveform(channel, waveform);
}

void TempestIOStreamProxy::SetTime(uint64_t clockCycles)
{
   // increment our clock
   cpuTime += clockCycles;

   // send a packet if it's time
   if ((cpuTime - lastSendTime) > 3000)
   {
      lastSendTime += 3000;
      currentState.SetElapsedTicks(6);

      stream.StartPacket();
      const uint8_t* packet = currentState.GetPacketData();
      int packetLength = currentState.GetLength();
      for (int i = 0; i < packetLength; ++i)
         stream.Write(packet[i]);
      stream.EndPacket();
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
   currentState.SetButtonLED(button, value);
}
