
#include "TempestDisco.h"

#include "AudioDriver.h"

#include "DiscoWaveStreamer.h"

DiscoWaveStreamer DWS;


DiscoWaveStreamer::DiscoWaveStreamer(void)
	:
		WaveStreamer(buffer, TEMPEST_DISCO_SOUND_BUFFER_SAMPLE_COUNT)
{

}


