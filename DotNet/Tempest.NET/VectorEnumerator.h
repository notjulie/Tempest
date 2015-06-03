
#ifndef VECTORENUMERATOR_H
#define VECTORENUMERATOR_H

using namespace System;
using namespace System::Runtime::InteropServices;

#include "TempestIO/Vector/SimpleVector.h"

class SimpleVectorDataInterpreter;
class VectorData;

public ref class VectorEnumerator
{
public:
	VectorEnumerator(const std::vector<SimpleVector> &vectorList);
	virtual ~VectorEnumerator(void);

	bool GetNextVector([Out] int16_t %startX, [Out] int16_t %startY, [Out] int16_t %endX, [Out] int16_t %endY, [Out] int %color);

private:
	std::vector<SimpleVector> *vectorData;
	int nextVector;
};

#endif
