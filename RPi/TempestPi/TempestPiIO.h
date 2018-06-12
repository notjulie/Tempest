
#ifndef TEMPESTPIIO_H
#define TEMPESTPIIO_H

#include "PiScreen.h"

class VectorGame;

class TempestPiIO
{
public:
   TempestPiIO(void);

   void Run(VectorGame *game);

   void LogFrameRate(void) { logFrameRate = true; }
   void Terminate(void) { terminated = true; }

private:
   void PushFrameToScreen(void);

private:
   VectorGame *game = nullptr;
	PiScreen screen;
	bool logFrameRate = false;
	bool terminated = false;
};

#endif

