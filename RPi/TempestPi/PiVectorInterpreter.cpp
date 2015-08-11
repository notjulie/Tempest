
#include "stdafx.h"
#include "PiVectorInterpreter.h"

PiVectorInterpreter::PiVectorInterpreter(void)
{
	x = y = 0;
	nextIndex = 0;
	color = 0;
	binaryScale = 1;
	linearScale = 0;
}


bool PiVectorInterpreter::GetNextVector(PiVector &_vector)
{
	if (nextIndex >= (int)vectors.size())
		return false;

	_vector = vectors[(unsigned)(nextIndex++)];
	return true;
}

void PiVectorInterpreter::Center(void)
{
	x = y = 0;
}

void PiVectorInterpreter::LDraw(int _x, int _y, int _intensity)
{
	// calculate the XY displacement
	int actualDX = (_x << (15 - binaryScale)) / (128 + linearScale);
	int actualDY = (_y << (15 - binaryScale)) / (128 + linearScale);

	// calculate the line endpoints extended to 32 bit
	int startX = x;
	int startY = y;
	int endX = x + actualDX;
	int endY = y + actualDY;

	// update our position
	x += actualDX;
	y += actualDY;

	// if the color is zero never mind
	if (_intensity == 0)
		return;

	// we need to clip the line to the bounds of signed 16 bits
	if (!ClipEndPoint(startX, startY, endX, endY))
		return;
	if (!ClipEndPoint(endX, endY, startX, startY))
		return;

	// add the vector to the list
	PiVector vector;
	vector.startX = startX;
	vector.startY = startY;
	vector.endX = endX;
	vector.endY = endY;
	vector.color = color;
	vectors.push_back(vector);
}

void PiVectorInterpreter::SDraw(int x, int y, int intensity)
{
	LDraw(x, y, intensity);
}


void PiVectorInterpreter::Stat(int color, int)
{
	this->color = color;
}

void PiVectorInterpreter::Scale(int binaryScale, int linearScale)
{
	this->binaryScale = binaryScale;
	this->linearScale = linearScale;
}

bool PiVectorInterpreter::ClipEndPoint(int &startX, int &startY, int &endX, int &endY)
{
	if (endX < -32768)
	{
		if (startX < -32768)
			return false;
		endY = startY + (endY - startY) * (-32768 - startX) / (endX - startX);
		endX = -32768;
	}

	if (endY < -32768)
	{
		if (startY < -32768)
			return false;
		endX = startX + (endX - startX) * (-32768 - startY) / (endY - startY);
		endY = -32768;
	}

	if (endX > 32767)
	{
		if (startX > 32767)
			return false;
		endY = startY + (endY - startY) * (32767 - startX) / (endX - startX);
		endX = 32767;
	}

	if (endY > 32767)
	{
		if (startY > 32767)
			return false;
		endX = startX + (endX - startX) * (32767 - startY) / (endY - startY);
		endY = 32767;
	}

	return true;
}


