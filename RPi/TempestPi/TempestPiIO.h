
#ifndef TEMPESTPIIO_H
#define TEMPESTPIIO_H


class TempestPiIO : public AbstractTempestIO
{
public:
   TempestPiIO() { isVectorHalt = true; }

	virtual void SetSoundChannelFrequency(int channel, int frequency) {}
	virtual void SetSoundChannelVolume(int channel, int volume) {}
	virtual void SetSoundChannelWaveform(int channel, int waveform) {}
	virtual void Tick6KHz(void) {}

	virtual void WriteVectorRAM(uint16_t address, uint8_t value) {}
	virtual bool IsVectorHalt(void) { return isVectorHalt; }
	virtual void VectorGo(void) { isVectorHalt = false; }
	virtual void VectorReset(void) { isVectorHalt = true; }

private:
   bool isVectorHalt;
};

#endif

