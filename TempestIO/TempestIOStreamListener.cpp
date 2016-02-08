
#include "stdafx.h"

#include "AbstractTempestIO.h"
#include "AbstractTempestStream.h"
#include "SoundIOPacket.h"
#include "TempestStreamProtocol.h"

#include "TempestIOStreamListener.h"


TempestIOStreamListener::TempestIOStreamListener(AbstractTempestStream *_stream, AbstractTempestSoundIO *tempestIO)
   :
   stream(_stream)
{
   // copy parameters
   this->stream = stream;
   this->tempestIO = tempestIO;

   // clear
   encoder = 0;
   buttons = 0;
   cpuTime = 0;
}

void TempestIOStreamListener::Service(void)
{
   // send a message with control panel information if anything
	// has changed
	uint8_t newButtons = tempestIO->GetButtons();
	uint8_t newEncoder = tempestIO->GetEncoder();
	if (newButtons!=buttons || newEncoder!=encoder)
   {
      TempestInPacket packet;
      packet.flags1 = newButtons;
      buttons = newButtons;

      int8_t encoderChange = (int8_t)(newEncoder - encoder);
      if (encoderChange > 0)
      {
         packet.flags1 |= ENCODER_UP;
         ++encoder;
      }
      else if (encoderChange < 0)
      {
         packet.flags1 |= ENCODER_DOWN;
         --encoder;
      }

      stream.StartPacket();
      stream.Write(packet.flags1);
      stream.EndPacket();
   }

	// process any new data on the stream
   for (;;)
   {
      // read a packet from the stream
      uint8_t packetBuffer[SoundIOPacket::PacketLength];
      int packetLength = stream.ReadPacket(packetBuffer, sizeof(packetBuffer));
      if (packetLength != SoundIOPacket::PacketLength)
         break;
      SoundIOPacket packet(packetBuffer);

      // send the packet data to the I/O module
      cpuTime += 3000;
      tempestIO->SetTime(cpuTime);

      for (int channel = 0; channel < 8; ++channel)
      {
         tempestIO->SetSoundChannelFrequency(channel, packet.GetSoundChannelFrequency(channel));
         tempestIO->SetSoundChannelWaveform(channel, packet.GetSoundChannelWaveform(channel));
         tempestIO->SetSoundChannelVolume(channel, packet.GetSoundChannelVolume(channel));
      }

      tempestIO->SetButtonLED(ONE_PLAYER_BUTTON, packet.GetButtonLED(ONE_PLAYER_BUTTON));
      tempestIO->SetButtonLED(TWO_PLAYER_BUTTON, packet.GetButtonLED(TWO_PLAYER_BUTTON));
   }
}
