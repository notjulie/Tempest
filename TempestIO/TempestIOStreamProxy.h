
#ifndef TEMPESTIOSTREAMPROXY_H
#define TEMPESTIOSTREAMPROXY_H

#include "AbstractTempestIO.h"

class AbstractTempestStream;

class TempestIOStreamProxy : public AbstractTempestSoundIO
{
public:
   TempestIOStreamProxy(AbstractTempestStream *stream);

	virtual void SetSoundChannelFrequency(int channel, int frequency);
	virtual void SetSoundChannelVolume(int channel, int volume);
	virtual void SetSoundChannelWaveform(int channel, int waveform);
	virtual void Tick6KHz(void);
   virtual uint8_t GetButtons(void) { return buttons; }

private:
   AbstractTempestStream	*stream;
   uint8_t buttons;
};

#endif
