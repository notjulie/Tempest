
#ifndef VECTORENUMERATOR_H
#define VECTORENUMERATOR_H

using namespace System;
using namespace System::Runtime::InteropServices;

#include "DisplayVector.h"

class SimpleVectorDataInterpreter;
class VectorData;

public ref class VectorEnumerator
{
public:
	VectorEnumerator(const std::vector<DisplayVector> &vectorList);
	virtual ~VectorEnumerator(void);

	bool GetNextVector(
      [Out] int16_t %startX,
      [Out] int16_t %startY,
      [Out] int16_t %endX,
      [Out] int16_t %endY,
      [Out] uint8_t %r,
      [Out] uint8_t %g,
      [Out] uint8_t %b
      );

private:
	std::vector<DisplayVector> *vectorData;
	int nextVector;
};

#endif
