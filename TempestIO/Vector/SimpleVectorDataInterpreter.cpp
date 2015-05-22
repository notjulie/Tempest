
#include "stdafx.h"
#include "SimpleVectorDataInterpreter.h"


SimpleVectorDataInterpreter::SimpleVectorDataInterpreter(void)
{
	x = y = 0;
	nextIndex = 0;
	color = 0;
	binaryScale = 1;
	linearScale = 0;
}


bool SimpleVectorDataInterpreter::GetNextVector(SimpleVector &_vector)
{
	if (nextIndex >= (int)vectors.size())
		return false;

	_vector = vectors[(unsigned)(nextIndex++)];
	return true;
}

void SimpleVectorDataInterpreter::Center(void)
{
	x = y = 0;
}

void SimpleVectorDataInterpreter::LDraw(int _x, int _y, int _intensity)
{
	float actualDX = (float)_x / (1 << binaryScale) * 256 / (128 + linearScale);
	float actualDY = (float)_y / (1 << binaryScale) * 256 / (128 + linearScale);

	if (_intensity != 0)
	{
		SimpleVector vector;
		vector.startX = x;
		vector.startY = y;
		vector.endX = x + actualDX;
		vector.endY = y + actualDY;
		vector.color = color;
		vectors.push_back(vector);
	}

	x += actualDX;
	y += actualDY;
}

void SimpleVectorDataInterpreter::SDraw(int x, int y, int intensity)
{
	LDraw(x, y, intensity);
}


void SimpleVectorDataInterpreter::Stat(int color, int)
{
	this->color = color;
}

void SimpleVectorDataInterpreter::Scale(int binaryScale, int linearScale)
{
	this->binaryScale = binaryScale;
	this->linearScale = linearScale;
}
