
#include "SimpleVectorDataInterpreter.h"


SimpleVectorDataInterpreter::SimpleVectorDataInterpreter(const VectorData &_data)
	: VectorDataInterpreter(_data)
{
	nextIndex = 0;
}


bool SimpleVectorDataInterpreter::GetNextVector(SimpleVector &_vector)
{
	if (nextIndex >= vectors.size())
		return false;

	_vector = vectors[nextIndex++];
	return true;
}

void SimpleVectorDataInterpreter::Center(void)
{
	x = y = 0;
}

void SimpleVectorDataInterpreter::LDraw(int _x, int _y, int _intensity)
{
	if (_intensity != 0)
	{
		SimpleVector vector;
		vector.startX = x;
		vector.startY = y;
		vector.endX = x + _x;
		vector.endY = y + _y;
		vector.color = _intensity;
		vectors.push_back(vector);
	}

	x += _x;
	y += _y;
}

void SimpleVectorDataInterpreter::SDraw(int x, int y, int intensity)
{
	LDraw(x, y, intensity);
}

