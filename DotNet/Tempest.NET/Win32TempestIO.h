
#ifndef WIN32TEMPESTIO_H
#define WIN32TEMPESTIO_H

#include "TempestIO/AbstractTempestIO.h"
#include "TempestIO/Vector/SimpleVectorDataInterpreter.h"

#include "Win32WaveStreamer.h"

struct SimpleVector;

class Win32TempestIO : public AbstractTempestIO
{
public:
	Win32TempestIO(void);
	virtual ~Win32TempestIO(void);

	void GetVectorList(std::vector<SimpleVector> &_vectorList);

	virtual void SetSoundChannelFrequency(int channel, int frequency);
	virtual void SetSoundChannelVolume(int channel, int volume);
	virtual void SetSoundChannelWaveform(int channel, int waveform);
	virtual void Tick6KHz(void);

	virtual void WriteVectorRAM(uint16_t address, uint8_t value) { vectorInterpreter.WriteVectorRAM(address, value); }
	virtual bool IsVectorHalt(void) { return vectorInterpreter.IsHalt(); }
	virtual void VectorGo(void) { vectorInterpreter.Go(); }
	virtual void VectorReset(void) { vectorInterpreter.Reset(); }

private:
	void VectorThread(void);

private:
	static long VectorThreadEntry(Win32TempestIO *pThis) { pThis->VectorThread(); return 0; }

private:
	Win32WaveStreamer	waveStreamer;

	HANDLE vectorThread;
	HANDLE vectorGeneratorMutex;
	DWORD vectorThreadID;
	bool terminating;
	SimpleVectorDataInterpreter	vectorInterpreter;
	std::vector<SimpleVector> currentVectors;
};


#endif
