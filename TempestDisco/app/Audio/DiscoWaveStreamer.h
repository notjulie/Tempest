// ====================================================================
// DiscoWaveStreamer.h
//    Author: Randy Rasmussen
//    Copyright: none... do what you will
//    Warranties: none... do what you will at your own risk
//
// File summary:
//    Contains the implementation of class DiscoWaveStreamer.  The parent
//    class, WaveStreamer, is a simple sound generator based originally
//    on simulating the Atari POKEY chip's sound generation.  Our job,
//    as the child class is to wait for the hardware to be ready to
//    output some sound and then stream some sound data from the base
//    class to the sound hardware.
// ====================================================================


#ifndef DISCOWAVESTREAMER_H
#define DISCOWAVESTREAMER_H

#include "TempestIO/WaveStreamer.h"
#include "AudioDriver.h"

/// <summary>
/// class derived from WaveStreamer that connects the streamed wave
/// data to the Discovery board's I2S sound output buffer.
/// </summary>
class DiscoWaveStreamer : public WaveStreamer
{
public:
	DiscoWaveStreamer(void);

	void Service(void);

private:
	int16_t buffer[TEMPEST_DISCO_SOUND_BUFFER_SAMPLE_COUNT];
};

extern DiscoWaveStreamer DWS;

#endif
