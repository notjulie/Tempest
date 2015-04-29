
#ifndef VECTORDATAINTERPRETER_H
#define VECTORDATAINTERPRETER_H

#include <stdint.h>
#include <vector>

#include "VectorData.h"

class VectorDataInterpreter
{
public:
	VectorDataInterpreter(const VectorData &_data);
	virtual ~VectorDataInterpreter(void);

	void Interpret(void);

	virtual void Center(void) {}
	virtual void LDraw(int x, int y, int intensity) {}
	virtual void Scale(int thing1, int thing2) {}
	virtual void SDraw(int x, int y, int intensity) {}
	virtual void Stat(int thing1, int thing2) {}

private:
	uint8_t GetAt(uint16_t pcOffset);
	bool SingleStep(void);

private:
	VectorData vectorData;
	uint16_t PC;
	std::vector<uint16_t> stack;
};

#endif
