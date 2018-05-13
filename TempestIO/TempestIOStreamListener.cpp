
#include "stdafx.h"

#include "AbstractTempestIO.h"
#include "AbstractTempestStream.h"
#include "SoundIOPacket.h"

#include "TempestIOStreamListener.h"


TempestIOStreamListener::TempestIOStreamListener(
         AbstractTempestStream *_stream,
         AbstractTempestSoundOutput *soundOutput,
         AbstractArcadeGameControlPanelReader *controlPanel)
   :
   stream(_stream)
{
   // copy parameters
   this->stream = stream;
   this->soundOutput = soundOutput;
   this->controlPanel = controlPanel;
}

void TempestIOStreamListener::Service(void)
{
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
      controlPanel->SetButtonLED(ONE_PLAYER_BUTTON, packet.GetButtonLED(ONE_PLAYER_BUTTON));
      controlPanel->SetButtonLED(TWO_PLAYER_BUTTON, packet.GetButtonLED(TWO_PLAYER_BUTTON));

      // send the initial sound channel data to the I/O module
      for (int channel = 0; channel < 8; ++channel)
         soundOutput->SetSoundChannelState(channel, packet.GetInitialSoundChannelState(channel));

      // add additional sound commands
      int ticksThisPacket = 0;
      {
         uint8_t channel;
         SoundChannelState state;
         uint8_t delay;
         while (packet.GetSoundCommand(&delay, &channel, &state))
         {
            ticksThisPacket += delay;
            soundOutput->SetTime(lastPacketCpuTime + ticksThisPacket * SoundIOPacketReader::ClockCyclesPerTick);
            soundOutput->SetSoundChannelState(channel, state);
         }
      }

      // update our time
      lastPacketCpuTime += SoundIOPacketReader::ClockCyclesPerPacket;
      soundOutput->SetTime(lastPacketCpuTime);
   }

   // send a message with control panel information if anything
   // has changed or enough time has passed
   TempestInPacket packet;
   packet.SetButtons(controlPanel->GetButtons());
   packet.SetEncoder(controlPanel->GetEncoder());
   if (packet != lastResponsePacket || (lastPacketCpuTime - lastResponsePacketTime) >= SoundIOPacketReader::MaxResponsePacketPeriod)
   {
      packet.WriteToStream(&stream);
      lastResponsePacket = packet;
      lastResponsePacketTime = lastPacketCpuTime;
   }
}
