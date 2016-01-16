
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

	virtual void WriteColorRAM(uint16_t address, uint8_t value) { vectorInterpreter.WriteColorRAM(address, value); }
	virtual void WriteVectorRAM(uint16_t address, uint8_t value) { vectorInterpreter.WriteVectorRAM(address, value); }
	virtual bool IsVectorHalt(void) { return vectorInterpreter.IsHalt(); }
	virtual void VectorGo(void) { vectorInterpreter.Go(); }
	virtual void VectorReset(void) { vectorInterpreter.Reset(); }

private:
   void PushFrameToScreen(void);

private:
	SimpleVectorDataInterpreter	vectorInterpreter;
	PiScreen screen;
	bool logFrameRate;
};

#endif

