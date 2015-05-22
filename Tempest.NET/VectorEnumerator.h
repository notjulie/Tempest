
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

	bool GetNextVector([Out] float %startX, [Out] float %startY, [Out] float %endX, [Out] float %endY, [Out] int %color);

private:
	std::vector<SimpleVector> *vectorData;
	int nextVector;
};

#endif
