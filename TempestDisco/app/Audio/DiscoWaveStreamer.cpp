
#include "TempestDisco.h"

#include "AudioDriver.h"
#include "SystemError.h"

#include "DiscoWaveStreamer.h"

DiscoWaveStreamer DWS;


DiscoWaveStreamer::DiscoWaveStreamer(void)
	:
		WaveStreamer(TEMPEST_DISCO_SOUND_BUFFER_SAMPLE_COUNT)
{

}


void WaveStreamer::ReportAllocError(void)
{
	ReportSystemError(SYSTEM_ERROR_WAVE_STREAMER_ALLOC_ERROR);
}
