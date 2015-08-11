
#include "stdafx.h"
#include "VG/vgu.h"

#include "PiScreen.h"

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
	PiVector vector(
      startX,
      startY,
      endX,
      endY,
      color
      );
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




PiVector::PiVector(void)
{
   startX = 0;
   startY = 0;
   endX = 0;
   endY = 0;
   color = 0;
}

PiVector::PiVector(
      int16_t startX,
      int16_t startY,
      int16_t endX,
      int16_t endY,
      int color
      )
{
   this->startX = startX;
   this->startY = startY;
   this->endX = endX;
   this->endY = endY;
   this->color = color;
}

void PiVector::Display(PiScreen *screen) const
{
   // create a path
   VGPath path = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL);

   // calculate our screen coordinates
   float x1 = (float)(startX + 32768) * screen->GetHeight() / 65536;
   float y1 = (float)(startY + 32768) * screen->GetHeight() / 65536;
   float x2 = (float)(endX + 32768) * screen->GetHeight() / 65536;
   float y2 = (float)(endY + 32768) * screen->GetHeight() / 65536;

   // if this is just a dot widen it to pixel size
   if (startX==endX && startY==endY)
   {
      x1 -= 0.5F;
      x2 += 0.5F;
   }

   // add the line to the path
   VGfloat poly[4] = {x1, y1, x2, y2};
   vguPolygon(path, poly, 2, false);

   // set the current color
   screen->SetColor(color);

   // draw
   vgSetf(VG_STROKE_LINE_WIDTH, 1);
   vgSeti(VG_STROKE_CAP_STYLE, VG_CAP_BUTT);
   vgSeti(VG_STROKE_JOIN_STYLE, VG_JOIN_MITER);
   vgDrawPath(path, VG_STROKE_PATH);

   // close the path
   vgDestroyPath(path);
}
