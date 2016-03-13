
#ifndef TEMPESTPIIO_H
#define TEMPESTPIIO_H

#include "TempestIO/AbstractTempestIO.h"
#include "TempestIO/Vector/SimpleVectorDataInterpreter.h"

#include "PiScreen.h"

class TempestPiIO : public AbstractTempestVectorIO
{
public:
   TempestPiIO(void);

   void Run(void);

   void LogFrameRate(void) { logFrameRate = true; }
   void Terminate(void) { terminated = true; }

	virtual void WriteColorRAM(uint16_t address, uint8_t value) { vectorInterpreter.WriteColorRAM(address, value); }
	virtual void WriteVectorRAM(uint16_t address, uint8_t value, uint64_t cpuTime);
	virtual bool IsVectorHalt(void);
	virtual void VectorGo(void);
	virtual void VectorReset(void);

private:
   void ProcessVectorData(void);
   void PushFrameToScreen(void);

private:
   typedef std::vector<SimpleVector> VectorList;

private:
	SimpleVectorDataInterpreter	vectorInterpreter;
	std::vector<VectorList> vectorLists;
	int mostRecentVectors;
	PiScreen screen;
	bool logFrameRate;
	bool terminated;
	uint64_t vectorGoCount;
	uint64_t vectorResetCount;
	uint64_t lastVectorRAMWrite;

	bool isGo;
	bool isHalt;
};

#endif

