
#ifndef TEMPESTPI_H
#define TEMPESTPI_H

class TempestPi {
public:
   TempestPi(void);

   void Run(void);
   void SetDemoMode(bool isDemo) { demo = isDemo; }

private:
   bool demo;
};

#endif
