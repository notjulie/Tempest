
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

      AbstractTempestSoundIO *GetSoundIOObject(void) { return tempestSoundIO; }
      void Fire(bool state) { tempestSoundIO->Fire(state); }
      void OnePlayer(bool state) { tempestSoundIO->OnePlayer(state); }
      void Zap(bool state) { tempestSoundIO->Zap(state); }
      void MoveWheel(int distance) { tempestSoundIO->MoveWheel(distance); }

   private:
      Win32TempestSoundIO *tempestSoundIO;
   };


   public ref class TDNWin32TempestVectorIO {
   public:
      TDNWin32TempestVectorIO(void);
      ~TDNWin32TempestVectorIO(void);

      VectorEnumerator ^GetVectorEnumerator(void);

      AbstractTempestVectorIO *GetVectorIOObject(void) { return tempestVectorIO; }

   private:
      Win32TempestVectorIO *tempestVectorIO;
   };

};

#endif
