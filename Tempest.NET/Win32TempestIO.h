
#ifndef WIN32TEMPESTIO_H
#define WIN32TEMPESTIO_H

#include "TempestIO/AbstractTempestIO.h"
#include "TempestIO/Vector/VectorGenerator.h"

#include "Win32WaveStreamer.h"


class Win32TempestIO : public AbstractTempestIO
{
public:
	virtual void SetSoundChannelFrequency(int channel, int frequency);
	virtual void SetSoundChannelVolume(int channel, int volume);
	virtual void SetSoundChannelWaveform(int channel, int waveform);
	virtual void Tick6KHz(void);

	virtual bool HaveNewVectorData(void) { return vectorGenerator.HaveNewData(); }
	virtual void PopVectorData(VectorData &_vectorData) { vectorGenerator.Pop(_vectorData); }
	virtual void LoadVectorROM(uint16_t address, const uint8_t *buffer, int count) { vectorGenerator.LoadROM(address, buffer, count); }
	virtual uint8_t	ReadVectorRAM(uint16_t address) { return vectorGenerator.ReadVectorRAM(address); }
	virtual uint8_t	ReadVectorROM(uint16_t address) { return vectorGenerator.ReadVectorROM(address); }
	virtual void		WriteVectorRAM(uint16_t address, uint8_t value) { vectorGenerator.WriteVectorRAM(address, value); }
	virtual bool IsVectorHalt(void) { return vectorGenerator.IsHalt(); }
	virtual void VectorGo(void) { vectorGenerator.Go(); }
	virtual void VectorReset(void) { vectorGenerator.Reset(); }

private:
	Win32WaveStreamer	waveStreamer;
	VectorGenerator vectorGenerator;
};


#endif
