
#ifndef TEMPESTPI_H
#define TEMPESTPI_H

#include "AbstractGameEnvironment.h"
#include "TempestDB.h"
#include "TempestSocketListener.h"
#include "TempestPiIO.h"

class PiSerialStream;
class TempestIOStreamProxy;
class VectorGame;
class VectorGameRunner;


class TempestPi {
public:
   TempestPi(void);
   ~TempestPi(void);

   void Run(void);

private:
   void Log(const char *s);
   void MonitorThread(void);

private:
   bool terminated = false;
   std::thread *monitorThread = nullptr;
   FILE *log = nullptr;
   char currentCommand [100];

   AbstractGameEnvironment environment;
   VectorGame *game = nullptr;
   VectorGameRunner *gameRunner = nullptr;
   TempestPiIO vectorIO;
   TempestSocketListener *socketListener = nullptr;
   PiSerialStream *serialStream = nullptr;
   TempestIOStreamProxy *soundIO = nullptr;
   TempestDB tempestDB;
};

#endif
