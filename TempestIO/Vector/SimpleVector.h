
#ifndef SIMPLEVECTOR_H
#define SIMPLEVECTOR_H

#include "TempestColor.h"

#ifdef _WIN32
   #pragma warning(push)
   #pragma warning(disable : 4820)	// padding in structures
#endif

struct SimpleVector
{
	int16_t startX;
	int16_t startY;
	int16_t endX;
	int16_t endY;
   TempestColor color;
};

#ifdef _WIN32
   #pragma warning(pop)
#endif

#endif
