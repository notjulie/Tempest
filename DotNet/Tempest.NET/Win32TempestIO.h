
#ifndef WIN32TEMPESTIO_H
#define WIN32TEMPESTIO_H

#include "TempestIO/AbstractSoundGenerator.h"
#include "TempestIO/AbstractTempestIO.h"

#include "Win32WaveStreamer.h"


class Win32TempestSoundIO :
   public AbstractSoundGenerator,
   public SimpleArcadeGameControlPanel
{
public:
   Win32TempestSoundIO(void);
   virtual ~Win32TempestSoundIO(void);

   virtual void SetSoundChannelState(int channel, SoundChannelState state);

protected:
   virtual void ProcessTimeLapse(uint64_t clockCycles);

private:
   Win32WaveStreamer	waveStreamer;
};


#endif
