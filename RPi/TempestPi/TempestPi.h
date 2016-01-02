
#ifndef TEMPESTPI_H
#define TEMPESTPI_H

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
};

#endif
