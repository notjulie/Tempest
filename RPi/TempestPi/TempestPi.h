
#ifndef TEMPESTPI_H
#define TEMPESTPI_H

#include "TempestCPU/TempestRunner.h"
#include "TempestPiEnvironment.h"
#include "TempestPiIO.h"


class TempestPi {
public:
   TempestPi(void);
   ~TempestPi(void);

   void Run(void);
   void SetDemoMode(bool isDemo) { demo = isDemo; }

private:
   void Log(const char *s);
   void MonitorThread(void);
   void KeyboardThread(void);
   void ProcessCommand(const char *command);

private:
   static void *KeyboardThreadEntry(void *pThis);
   static void *MonitorThreadEntry(void *pThis);

private:
   bool demo;
   bool terminated;
   pthread_t monitorThread;
   pthread_t keyboardThread;
   FILE *log;
   char currentCommand [100];

   TempestPiEnvironment environment;
   TempestRunner tempestRunner;
   TempestPiIO vectorIO;
};

#endif
