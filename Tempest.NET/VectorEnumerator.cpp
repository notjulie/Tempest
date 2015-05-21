
#include "stdafx.h"

#include "TempestCPU/Vector/SimpleVectorDataInterpreter.h"
#include "TempestIO/Vector/VectorData.h"

#include "VectorEnumerator.h"


VectorEnumerator::VectorEnumerator(const VectorData &_vectorData)
{
	vectorData = new VectorData(_vectorData);
	interpreter = NULL;
}

VectorEnumerator::~VectorEnumerator(void)
{
	delete vectorData, vectorData = NULL;
	delete interpreter, interpreter = NULL;
}

bool VectorEnumerator::GetNextVector([Out] float %startX, [Out] float %startY, [Out] float %endX, [Out] float %endY, [Out] int %color)
{
	// interpret if we haven't
	if (interpreter == NULL)
	{
		interpreter = new SimpleVectorDataInterpreter(*vectorData);
		interpreter->Interpret();
	}

	SimpleVector	vector;
	if (!interpreter->GetNextVector(vector))
		return false;

	startX = vector.startX;
	startY = vector.startY;
	endX =   vector.endX;
	endY =   vector.endY;
	color =  vector.color;
	return true;
}
