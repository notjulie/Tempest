
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

      void Fire(bool state) { tempestSoundIO->SetInputButtonState(FIRE_BUTTON, state); }
      void OnePlayer(bool state) { tempestSoundIO->SetInputButtonState(ONE_PLAYER_BUTTON, state); }
      void TwoPlayer(bool state) { tempestSoundIO->SetInputButtonState(TWO_PLAYER_BUTTON, state); }
      void Zap(bool state) { tempestSoundIO->SetInputButtonState(ZAPPER_BUTTON, state); }
      void MoveWheel(int distance) { tempestSoundIO->MoveSpinner(distance); }
      bool OnePlayerLED(void) { return tempestSoundIO->GetPlayer1LEDState(); }
      bool TwoPlayerLED(void) { return tempestSoundIO->GetPlayer2LEDState(); }

   private:
      Win32TempestSoundIO *tempestSoundIO;
   };


};

#endif
