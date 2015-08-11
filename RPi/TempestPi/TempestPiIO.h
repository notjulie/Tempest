
#ifndef TEMPESTPIIO_H
#define TEMPESTPIIO_H

#include "TempestIO/AbstractTempestIO.h"

#include "PiScreen.h"
#include "PiVectorInterpreter.h"


class TempestPiIO : public AbstractTempestIO
{
public:
   TempestPiIO() {}

   void PushFrameToScreen(void);

	virtual void SetSoundChannelFrequency(int channel, int frequency) {}
	virtual void SetSoundChannelVolume(int channel, int volume) {}
	virtual void SetSoundChannelWaveform(int channel, int waveform) {}
	virtual void Tick6KHz(void) {}

	virtual void WriteVectorRAM(uint16_t address, uint8_t value) { vectorInterpreter.WriteVectorRAM(address, value); }
	virtual bool IsVectorHalt(void) { return vectorInterpreter.IsHalt(); }
	virtual void VectorGo(void) { vectorInterpreter.Go(); }
	virtual void VectorReset(void) { vectorInterpreter.Reset(); }

private:
	PiVectorInterpreter	vectorInterpreter;
	PiScreen screen;
};

#endif

