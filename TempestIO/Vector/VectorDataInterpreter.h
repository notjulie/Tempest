
#ifndef VECTORDATAINTERPRETER_H
#define VECTORDATAINTERPRETER_H


#include "VectorData.h"

#define VECTOR_DATA_STACK_SIZE 100

class VectorDataInterpreter
{
public:
	VectorDataInterpreter(void);
	virtual ~VectorDataInterpreter(void);

	void Interpret(void);
   void SetVectorData(const VectorData &vectorData) { this->vectorData = vectorData; }
	void WriteVectorRAM(uint16_t address, uint8_t value) { vectorData.WriteVectorRAM(address, value); }
   void WriteColorRAM(uint16_t address, uint8_t value) { vectorData.WriteColorRAM(address, value); }
   uint8_t ReadColorRAM(uint16_t address) { return vectorData.ReadColorRAM(address); }
   bool IsHalt(void) { return isHalt; }
	void Go(void) { goRequested = true; }
	void Reset(void);
   void RegisterHook(uint16_t address, std::function<void()> hook);

   uint16_t GetPC(void) const { return PC; }
   void Jump(uint16_t pc) { PC = pc; }

protected:
	virtual void Center(void);
	virtual void LDraw(int x, int y, int intensity);
	virtual void Scale(int binaryScale, int linearScale);
	virtual void SDraw(int x, int y, int intensity);
	virtual void Stat(int color, int intensity);

private:
	uint8_t GetAt(uint16_t pcOffset);
   bool SingleStep(void);

private:
	VectorData vectorData;
	uint16_t PC;
   uint16_t stack[VECTOR_DATA_STACK_SIZE];
   int stackIndex;
	bool isHalt;
	bool resetRequested;
	bool goRequested;
   bool hookFlags[16 * 1024];
   std::map<uint16_t, std::function<void()> > hooks;
};


#endif
