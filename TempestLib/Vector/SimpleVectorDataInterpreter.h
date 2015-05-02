
#ifndef SIMPLEVECTORDATAINTERPRETER_H
#define SIMPLEVECTORDATAINTERPRETER_H

#include "SimpleVector.h"
#include "VectorDataInterpreter.h"

#pragma warning(push)
#pragma warning(disable : 4820)	// padding in structures

class SimpleVectorDataInterpreter : public VectorDataInterpreter
{
public:
	SimpleVectorDataInterpreter(const VectorData &_data);

	bool GetNextVector(SimpleVector &_vector);

protected:
	virtual void Center(void);
	virtual void LDraw(int x, int y, int intensity);
	virtual void SDraw(int x, int y, int intensity);

private:
	int nextIndex;
	std::vector<SimpleVector> vectors;
	float x, y;
};

#pragma warning(pop)

#endif
