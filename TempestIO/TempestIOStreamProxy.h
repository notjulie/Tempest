
#ifndef TEMPESTIOSTREAMPROXY_H
#define TEMPESTIOSTREAMPROXY_H

#include "AbstractTempestIO.h"

class AbstractTempestOutStream;

class TempestIOStreamProxy : public AbstractTempestIO
{
public:
   TempestIOStreamProxy(AbstractTempestOutStream *stream);

	virtual void SetSoundChannelFrequency(int channel, int frequency);
	virtual void SetSoundChannelVolume(int channel, int volume);
	virtual void SetSoundChannelWaveform(int channel, int waveform);
	virtual void Tick6KHz(void);

	virtual void WriteVectorRAM(uint16_t address, uint8_t value);
	virtual bool IsVectorHalt(void);
	virtual void VectorGo(void);
	virtual void VectorReset(void);

private:
   AbstractTempestOutStream	*stream;
   bool isVectorHalt;
};

#endif
