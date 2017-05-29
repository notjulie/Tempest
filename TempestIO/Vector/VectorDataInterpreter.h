
#ifndef VECTORDATAINTERPRETER_H
#define VECTORDATAINTERPRETER_H


#include "VectorData.h"


class VectorDataInterpreter
{
public:
	VectorDataInterpreter(void);
	virtual ~VectorDataInterpreter(void);

   void Interpret(void) { isHalt = false;  InterpretAt(0); }
   void SetVectorData(const VectorData &vectorData) { this->vectorData = vectorData; }
	void WriteVectorRAM(uint16_t address, uint8_t value) { vectorData.WriteVectorRAM(address, value); }
   void WriteColorRAM(uint16_t address, uint8_t value) { vectorData.WriteColorRAM(address, value); }
   uint8_t ReadColorRAM(uint16_t address) { return vectorData.ReadColorRAM(address); }
   void RegisterHook(uint16_t address, std::function<uint16_t(uint16_t)> hook);

protected:
	virtual void Center(void);
	virtual void LDraw(int x, int y, int intensity);
	virtual void Scale(int binaryScale, int linearScale);
	virtual void SDraw(int x, int y, int intensity);
	virtual void Stat(int color, int intensity);

protected:
   void InterpretAt(uint16_t pc);
   uint16_t SingleStep(uint16_t pc);

protected:
	VectorData vectorData;
	bool isHalt = false;
   bool hookFlags[16 * 1024];
   std::map<uint16_t, std::function<uint16_t(uint16_t)> > hooks;
};


#endif
