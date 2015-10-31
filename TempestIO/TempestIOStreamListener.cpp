
#include "stdafx.h"

#include "AbstractTempestIO.h"
#include "AbstractTempestStream.h"
#include "TempestStreamProtocol.h"

#include "TempestIOStreamListener.h"


TempestIOStreamListener::TempestIOStreamListener(AbstractTempestStream *stream, AbstractTempestSoundIO *tempestIO)
{
   // copy parameters
   this->stream = stream;
   this->tempestIO = tempestIO;

   // clear
   state = IDLE;
}

void TempestIOStreamListener::Service(void)
{
   // process any new data on the stream
   for (;;)
   {
      // get a byte from the stream
      int b = stream->Read();
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

            // this is our cue for a response packet
            {
               TempestInPacket packet;
               packet.flags1 = tempestIO->GetButtons();
               stream->Write(packet.flags1);
            }
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
      }
   }
}
