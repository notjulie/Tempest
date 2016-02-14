
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
   lastPacketCpuTime = 0;
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
      uint8_t packetBuffer[SoundIOPacketReader::MaxPacketLength];
      int packetLength = stream.ReadPacket(packetBuffer, sizeof(packetBuffer));
      if (packetLength <= 0)
         break;
      SoundIOPacketReader packet(packetBuffer, packetLength);

      // set the buttons
      tempestIO->SetButtonLED(ONE_PLAYER_BUTTON, packet.GetButtonLED(ONE_PLAYER_BUTTON));
      tempestIO->SetButtonLED(TWO_PLAYER_BUTTON, packet.GetButtonLED(TWO_PLAYER_BUTTON));

      // send the initial sound channel data to the I/O module
      for (int channel = 0; channel < 8; ++channel)
         tempestIO->SetSoundChannelState(channel, packet.GetInitialSoundChannelState(channel));

      // add additional sound commands
      int ticksThisPacket = 0;
      {
         uint8_t channel;
         SoundChannelState state;
         uint8_t delay;
         while (packet.GetSoundCommand(&delay, &channel, &state))
         {
            ticksThisPacket += delay;
            if (ticksThisPacket > SoundIOPacketReader::TicksPerPacket)
               throw "bleem";
            tempestIO->SetTime(lastPacketCpuTime + ticksThisPacket * SoundIOPacketReader::ClockCyclesPerTick);
            tempestIO->SetSoundChannelState(channel, state);
         }
      }

      // update our time
      lastPacketCpuTime += SoundIOPacketReader::ClockCyclesPerPacket;
      tempestIO->SetTime(lastPacketCpuTime);
   }
}
