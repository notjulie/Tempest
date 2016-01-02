
#ifndef TEMPESTPI_H
#define TEMPESTPI_H

#include "TempestCPU/TempestRunner.h"
#include "TempestPiEnvironment.h"

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
   static void *MonitorThreadEntry(void *pThis);

private:
   bool demo;
   bool terminated;
   pthread_t monitorThread;

   TempestPiEnvironment environment;
   TempestRunner tempestRunner;
};

#endif
