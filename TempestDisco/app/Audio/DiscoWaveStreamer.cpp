
#include "TempestDisco.h"

#include "AudioDriver.h"

#include "DiscoWaveStreamer.h"

DiscoWaveStreamer DWS;


DiscoWaveStreamer::DiscoWaveStreamer(void)
	:
		WaveStreamer(buffer, TEMPEST_DISCO_SOUND_BUFFER_SAMPLE_COUNT)
{
}


void DiscoWaveStreamer::Service(void)
{
   // see if we have a buffer ready to fill
   int16_t *bufferToFill;
   int frameCount;
   if (AudioDriverPopEmptyBuffer(&bufferToFill, &frameCount))
   {
   	// we need to convert mono to stereo... start by filling the
   	// buffer mono
      FillBuffer(bufferToFill, frameCount);

      // then make it stereo
      for (int i=frameCount-1; i>=0; --i)
      {
      	bufferToFill[i*2 + 1] = bufferToFill[i];
      	bufferToFill[i * 2] = bufferToFill[i];
      }
   }

	// if we still have events to process, process them
	while (ProcessNextEvent())
		continue;
}

