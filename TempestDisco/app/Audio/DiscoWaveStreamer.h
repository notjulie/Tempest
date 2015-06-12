
#ifndef DISCOWAVESTREAMER_H
#define DISCOWAVESTREAMER_H

#include "TempestIO/WaveStreamer.h"
#include "AudioDriver.h"

class DiscoWaveStreamer : public WaveStreamer
{
public:
	DiscoWaveStreamer(void);

private:
	int16_t buffer[TEMPEST_DISCO_SOUND_BUFFER_SAMPLE_COUNT];
};

extern DiscoWaveStreamer DWS;

#endif
