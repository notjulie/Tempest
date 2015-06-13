
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
      FillBuffer(bufferToFill, frameCount * 2);

	// if we still have events to process, process them
	while (ProcessNextEvent())
		continue;

	// set the blue LED if we have sound output
	GPIO_WriteBit(LED_BLUE_GPIO_PORT, LED_BLUE_PIN, HaveSoundOutput() ? Bit_SET : Bit_RESET);
}

