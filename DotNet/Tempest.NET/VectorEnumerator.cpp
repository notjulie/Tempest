
#include "stdafx.h"

#include "TempestIO/Vector/SimpleVectorDataInterpreter.h"
#include "TempestIO/Vector/VectorData.h"

#include "VectorEnumerator.h"


VectorEnumerator::VectorEnumerator(const std::vector<SimpleVector> &vectorList)
{
	vectorData = new std::vector<SimpleVector>(vectorList);
	nextVector = 0;
}

VectorEnumerator::~VectorEnumerator(void)
{
	delete vectorData, vectorData = NULL;
}

bool VectorEnumerator::GetNextVector(
   [Out] int16_t %startX,
   [Out] int16_t %startY,
   [Out] int16_t %endX,
   [Out] int16_t %endY,
   [Out] uint8_t %r,
   [Out] uint8_t %g,
   [Out] uint8_t %b
   )
{
	if (nextVector >= (int)vectorData->size())
		return false;

	SimpleVector	vector = (*vectorData)[nextVector++];
	startX = vector.startX;
	startY = vector.startY;
	endX =   vector.endX;
	endY =   vector.endY;
   r = vector.r;
   g = vector.g;
   b = vector.b;
   return true;
}
