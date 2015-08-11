
#include "stdafx.h"
#include "VG/vgu.h"

#include "PiScreen.h"

#include "PiVectorInterpreter.h"

PiVectorInterpreter::PiVectorInterpreter(void)
{
	x = y = 0;
	nextIndex = 0;
	color = 0;
	scale = 1.0f;
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
	// calculate the line endpoints
	float startX = x;
	float startY = y;
	float endX = x + scale * _x;
	float endY = y + scale * _y;

	// update our position
	x = endX;
	y = endY;

	// if the color is zero never mind
	if (_intensity == 0)
		return;

	// put together a vector
	PiVector vector(
      startX,
      startY,
      endX,
      endY,
      color
      );

   // add it to the list
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
   this->scale = (float)(1 << (8 - binaryScale)) / (128 + linearScale);
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
      float startX,
      float startY,
      float endX,
      float endY,
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
   float x1 = (float)(startX + 256) * screen->GetHeight() / 512;
   float y1 = (float)(startY + 256) * screen->GetHeight() / 512;
   float x2 = (float)(endX + 256) * screen->GetHeight() / 512;
   float y2 = (float)(endY + 256) * screen->GetHeight() / 512;

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
