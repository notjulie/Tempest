
#ifndef WIN32TEMPESTIO_H
#define WIN32TEMPESTIO_H

#include "TempestIO/AbstractTempestIO.h"
#include "Win32WaveStreamer.h"

class Win32TempestIO : public AbstractTempestIO
{
public:
	virtual void SetSoundChannelFrequency(int channel, int frequency);
	virtual void SetSoundChannelVolume(int channel, int volume);
	virtual void SetSoundChannelWaveform(int channel, int waveform);
	virtual void Tick6KHz(void);

private:
	Win32WaveStreamer	waveStreamer;
};


#endif
