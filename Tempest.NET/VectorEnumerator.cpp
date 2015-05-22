
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

bool VectorEnumerator::GetNextVector([Out] float %startX, [Out] float %startY, [Out] float %endX, [Out] float %endY, [Out] int %color)
{
	if (nextVector >= vectorData->size())
		return false;

	SimpleVector	vector = (*vectorData)[nextVector++];
	startX = vector.startX;
	startY = vector.startY;
	endX =   vector.endX;
	endY =   vector.endY;
	color =  vector.color;
	return true;
}
