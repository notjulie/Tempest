
#ifndef TEMPESTPI_H
#define TEMPESTPI_H

#include "TempestCPU/TempestRunner.h"
#include "TempestSocketListener.h"
#include "TempestPiEnvironment.h"
#include "TempestPiIO.h"

class PiSerialStream;
class TempestIOStreamProxy;


class TempestPi {
public:
   TempestPi(void);
   ~TempestPi(void);

   void Run(void);
   void SetDemoMode(bool isDemo) { demo = isDemo; }

private:
   void Log(const char *s);
   void MonitorThread(void);

private:
   bool demo;
   bool terminated;
   std::thread *monitorThread;
   FILE *log;
   char currentCommand [100];

   TempestPiEnvironment environment;
   TempestRunner *tempestRunner;
   TempestPiIO vectorIO;
   TempestSocketListener *socketListener;
   PiSerialStream *serialStream;
   TempestIOStreamProxy *soundIO;
};

#endif
