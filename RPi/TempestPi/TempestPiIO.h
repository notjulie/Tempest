
#ifndef TEMPESTPIIO_H
#define TEMPESTPIIO_H

#include "PiScreen.h"

class TempestRunner;

class TempestPiIO
{
public:
   TempestPiIO(void);

   void Run(TempestRunner *tempestRunner);

   void LogFrameRate(void) { logFrameRate = true; }
   void Terminate(void) { terminated = true; }

private:
   void PushFrameToScreen(void);

private:
   TempestRunner *tempestRunner;
	PiScreen screen;
	bool logFrameRate;
	bool terminated;
};

#endif

