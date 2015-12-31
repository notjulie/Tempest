
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
	void WriteVectorRAM(uint16_t address, uint8_t value) { vectorData.WriteVectorRAM(address, value); }
   void WriteColorRAM(uint16_t address, uint8_t value) { vectorData.WriteColorRAM(address, value); }
   bool IsHalt(void) { return isHalt; }
	void Go(void) { goRequested = true; }
	void Reset(void);

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
};


#endif
