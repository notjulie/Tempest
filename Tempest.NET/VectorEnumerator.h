
#ifndef VECTORENUMERATOR_H
#define VECTORENUMERATOR_H

using namespace System;
using namespace System::Runtime::InteropServices;

#include "TempestLib/Vector/SimpleVector.h"

class SimpleVectorDataInterpreter;
class VectorData;

public ref class VectorEnumerator
{
public:
	VectorEnumerator(const VectorData &vectorData);
	virtual ~VectorEnumerator(void);

	bool GetNextVector([Out] float %startX, [Out] float %startY, [Out] float %endX, [Out] float %endY, [Out] int %color);

private:
	VectorData *vectorData;
	SimpleVectorDataInterpreter *interpreter;
};

#endif
