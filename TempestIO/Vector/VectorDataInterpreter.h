
#ifndef VECTORDATAINTERPRETER_H
#define VECTORDATAINTERPRETER_H


#include "VectorData.h"

#pragma warning(push)
#pragma warning(disable : 4820)	// padding in structures

class VectorDataInterpreter
{
public:
	VectorDataInterpreter(void);
	virtual ~VectorDataInterpreter(void);

	void Interpret(void);
	void WriteVectorRAM(uint16_t address, uint8_t value) { vectorData.WriteVectorRAM(address, value); }
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
	std::vector<uint16_t> stack;
	bool isHalt;
	bool resetRequested;
	bool goRequested;
};

#pragma warning(pop)

#endif
