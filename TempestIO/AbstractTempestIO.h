
#ifndef ABSTRACTTEMPESTIO_H
#define ABSTRACTTEMPESTIO_H

class VectorData;

class AbstractTempestSoundIO
{
public:
   virtual ~AbstractTempestSoundIO(void) {}

	virtual void SetSoundChannelFrequency(int channel, int frequency) = 0;
	virtual void SetSoundChannelVolume(int channel, int volume) = 0;
	virtual void SetSoundChannelWaveform(int channel, int waveform) = 0;
	virtual void Tick6KHz(void) = 0;
};

class AbstractTempestVectorIO
{
public:
   virtual ~AbstractTempestVectorIO(void) {}

   virtual void WriteVectorRAM(uint16_t address, uint8_t value) = 0;
   virtual bool IsVectorHalt(void) = 0;
   virtual void VectorGo(void) = 0;
   virtual void VectorReset(void) = 0;
};

class AbstractTempestIO : public AbstractTempestSoundIO, public AbstractTempestVectorIO
{
};

#endif
