
#include "TempestIO.Headers.h"
#include <stdint.h>
#include "SimpleVectorDataInterpreter.h"


void SimpleVectorGenerator::Center(void)
{
	x = y = 0;
}

void SimpleVectorGenerator::LDraw(int _x, int _y, int _intensity)
{
	// calculate the XY displacement
	float actualDX = (float)(_x << (14 - binaryScale));
	actualDX /= (128 + linearScale);
   float actualDY = (float)(_y << (14 - binaryScale));
	actualDY /= (128 + linearScale);

	// some of the screens are wider than our 16-bit range that we clip to.
	// so downscale the X distance a little
	actualDX *= 0.93F;

	// calculate the line endpoints extended to 32 bit
	int startX = (int)x;
   int startY = (int)y;
   int endX = (int)(x + actualDX);
   int endY = (int)(y + actualDY);

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
   DisplayVector vector;
   vector.type = DisplayVector::Line;
	vector.line.startX = startX;
	vector.line.startY = startY;
	vector.line.endX = endX;
	vector.line.endY = endY;

   if ((color & 2) == 0)
      vector.line.r = 255;
   else if ((color & 1) == 0)
      vector.line.r = 128;
   else
      vector.line.r = 0;

   if ((color & 8) == 0)
      vector.line.g = 255;
   else
      vector.line.g = 0;

   if ((color & 4) == 0)
      vector.line.b = 255;
   else
      vector.line.b = 0;

   vectors.push_back(vector);
}

void SimpleVectorGenerator::SDraw(int x, int y, int intensity)
{
   // LDraw's units are double SDraw's
	LDraw(x * 2, y * 2, intensity);
}


void SimpleVectorGenerator::Stat(int colorIndex, int intensity, int color)
{
	this->color = color;
}

void SimpleVectorGenerator::Scale(int binaryScale, int linearScale)
{
	this->binaryScale = binaryScale;
	this->linearScale = linearScale;
}

bool SimpleVectorGenerator::ClipEndPoint(int &startX, int &startY, int &endX, int &endY)
{
	if (endX < -32768)
	{
		if (startX < -32768)
			return false;
        endY = (int)(startY + (int64_t)(endY - startY) * (-32768 - startX) / (endX - startX));
		endX = -32768;
	}

	if (endY < -32768)
	{
		if (startY < -32768)
			return false;
        endX = (int)(startX + (int64_t)(endX - startX) * (-32768 - startY) / (endY - startY));
		endY = -32768;
	}

	if (endX > 32767)
	{
		if (startX > 32767)
			return false;
        endY = (int)(startY + (int64_t)(endY - startY) * (32767 - startX) / (endX - startX));
		endX = 32767;
	}

	if (endY > 32767)
	{
		if (startY > 32767)
			return false;
        endX = (int)(startX + (int64_t)(endX - startX) * (32767 - startY) / (endY - startY));
		endY = 32767;
	}

	return true;
}

