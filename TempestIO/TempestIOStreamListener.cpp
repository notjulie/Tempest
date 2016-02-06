
#include "stdafx.h"

#include "AbstractTempestIO.h"
#include "AbstractTempestStream.h"
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
   state = IDLE;
   encoder = 0;
   buttons = 0;
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
      // get a byte from the stream
      int b = stream.Read();
      if (b < 0)
         break;

      // switch according to state
      switch (state)
      {
      case IDLE:
         switch (b>>5)
         {
         case OP_6KHZ_TICK:
            // tick
            tempestIO->Tick6KHz();
            break;

         case OP_BUTTON_LEDS:
            state = BUTTON_LEDS;
            break;

         case OP_SOUND_VOLUME:
            state = SOUND_VOLUME;
            soundChannel = b & 7;
            break;

         case OP_SOUND_WAVE:
            state = SOUND_WAVE;
            soundChannel = b & 7;
            break;

         case OP_SOUND_FREQUENCY:
            state = SOUND_FREQUENCY;
            soundChannel = b & 7;
            break;
         }
         break;

      case SOUND_FREQUENCY:
         tempestIO->SetSoundChannelFrequency(soundChannel, b);
         state = IDLE;
         break;

      case SOUND_VOLUME:
         tempestIO->SetSoundChannelVolume(soundChannel, b);
         state = IDLE;
         break;

      case SOUND_WAVE:
         tempestIO->SetSoundChannelWaveform(soundChannel, b);
         state = IDLE;
         break;

      case BUTTON_LEDS:
         tempestIO->SetButtonLED(ONE_PLAYER_BUTTON, (b&ONE_PLAYER_BUTTON) != 0);
         tempestIO->SetButtonLED(TWO_PLAYER_BUTTON, (b&TWO_PLAYER_BUTTON) != 0);
         state = IDLE;
         break;
      }
   }
}
