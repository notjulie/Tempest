
#ifndef ABSTRACTTEMPESTIO_H
#define ABSTRACTTEMPESTIO_H

class AbstractTempestIO
{
public:
	virtual void SetSoundChannelFrequency(int channel, int frequency) = 0;
	virtual void SetSoundChannelVolume(int channel, int frequency) = 0;
	virtual void SetSoundChannelWaveform(int channel, int frequency) = 0;
};

#endif
