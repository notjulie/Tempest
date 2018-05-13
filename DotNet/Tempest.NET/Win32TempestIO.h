
#ifndef WIN32TEMPESTIO_H
#define WIN32TEMPESTIO_H

#include "TempestIO/AbstractTempestIO.h"

#include "Win32WaveStreamer.h"


class Win32TempestSoundIO :
   public AbstractTempestSoundOutput,
   public SimpleArcadeGameControlPanel
{
public:
   Win32TempestSoundIO(void);
   virtual ~Win32TempestSoundIO(void);

   virtual void SetSoundChannelState(int channel, SoundChannelState state);
   virtual void SetTime(uint64_t clockCycles);

private:
   Win32WaveStreamer	waveStreamer;
   uint64_t cpuTime = 0;
};


#endif
