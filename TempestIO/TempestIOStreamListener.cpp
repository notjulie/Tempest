
#include "stdafx.h"

#include "AbstractTempestIO.h"
#include "AbstractTempestStream.h"
#include "TempestStreamProtocol.h"

#include "TempestIOStreamListener.h"


TempestIOStreamListener::TempestIOStreamListener(AbstractTempestStream *stream, AbstractTempestIO *tempestIO)
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

      // switch acording to state
      switch (state)
      {
      case IDLE:
         switch (b>>5)
         {
         case OP_VECTOR_RESET:
            tempestIO->VectorReset();
            break;

         case OP_6KHZ_TICK:
            tempestIO->Tick6KHz();
            break;

         case OP_VECTOR_GO:
            tempestIO->VectorGo();
            break;

         case OP_WRITE_VECTOR_RAM:
            state = VECTOR_RAM_ADDRESS_LOW;
            vectorRAMAddress = (b & 0xF)<<8;
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

         default:
            throw std::runtime_error("TempestIOStreamListener: unknown op");
         }
         break;

      case VECTOR_RAM_ADDRESS_LOW:
         state = VECTOR_RAM_VALUE;
         vectorRAMAddress += b;
         break;

      case VECTOR_RAM_VALUE:
         tempestIO->WriteVectorRAM(vectorRAMAddress, b);
         state = IDLE;
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

      default:
         throw std::runtime_error("TempestIOStreamListener: unknown state");
      }
   }
}
