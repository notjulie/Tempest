
#ifndef TEMPESTIODOTNET_H
#define TEMPESTIODOTNET_H

using namespace System;
using namespace System::Threading;

#include "VectorEnumerator.h"
#include "Win32TempestIO.h"

class Win32TempestIO;

namespace TempestDotNET {

   public ref class TDNWin32TempestSoundIO {
   public:
      TDNWin32TempestSoundIO(void);
      ~TDNWin32TempestSoundIO(void);

      AbstractTempestSoundOutput *GetSoundOutput(void) { return tempestSoundIO; }
      AbstractArcadeGameControlPanel *GetControlPanel(void) { return tempestSoundIO; }
      void Fire(bool state) { tempestSoundIO->Fire(state); }
      void OnePlayer(bool state) { tempestSoundIO->OnePlayer(state); }
      void TwoPlayer(bool state) { tempestSoundIO->TwoPlayer(state); }
      void Zap(bool state) { tempestSoundIO->Zap(state); }
      void MoveWheel(int distance) { tempestSoundIO->MoveWheel(distance); }
      bool OnePlayerLED(void) { return tempestSoundIO->OnePlayerLED(); }
      bool TwoPlayerLED(void) { return tempestSoundIO->TwoPlayerLED(); }

   private:
      Win32TempestSoundIO *tempestSoundIO;
   };


};

#endif
