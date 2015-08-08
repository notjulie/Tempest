
#include "stdafx.h"

#include "TempestCPU/AbstractTempestEnvironment.h"
#include "TempestCPU/TempestRunner.h"
#include "TempestIO/AbstractTempestIO.h"


class TempestPiEnvironment : public AbstractTempestEnvironment
{
public:
	virtual void Reset(void) {}
	virtual void Sleep(int ms) {}
	virtual void SynchronizeClock(uint64_t busMSCount) {}
};

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

int main()
{
    TempestPiEnvironment environment;
    TempestPiIO io;

    TempestRunner tempestRunner(&environment);
    tempestRunner.SetTempestIO(&io);

    tempestRunner.Start();
}
