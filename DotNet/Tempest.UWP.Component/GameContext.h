
#ifndef GAMECONTEXT_H
#define GAMECONTEXT_H

#include "AbstractSoundGenerator.h"
#include "AbstractTempestIO.h"
#include "SoundChannel.h"
#include "TempestMemoryStream.h"


/// <summary>
/// Abstract class for classes that want to provide the external connections
/// from the game to the world.
/// </summary>
class GameContext {
public:
   GameContext(void) {}
   virtual ~GameContext(void) {}

   AbstractSoundGenerator *GetSoundOutput(void) const { return soundOutput; }
   AbstractArcadeGameControlPanelReader *GetControlPanelReader(void) const { return controlPanelReader; }
   AbstractArcadeGameControlPanelWriter *GetControlPanelWriter(void) const { return controlPanelWriter; }

protected:
   AbstractSoundGenerator * soundOutput = nullptr;
   AbstractArcadeGameControlPanelReader *controlPanelReader = nullptr;
   AbstractArcadeGameControlPanelWriter *controlPanelWriter = nullptr;
};


class UWPSoundIO :
   public AbstractSoundGenerator,
   public SimpleArcadeGameControlPanel
{
public:
   UWPSoundIO(void) {}
   virtual ~UWPSoundIO(void) {}

   virtual void SetSoundChannelState(int channel, SoundChannelState state) {}

protected:
   virtual void ProcessTimeLapse(uint64_t clockCycles) {}

private:
   //Win32WaveStreamer	*waveStreamer = nullptr;
};


/// <summary>
/// GameContext implementation for games that want to connect straight to the Windows hardware.
/// </summary>
class NormalGameContext : public GameContext {
public:
   NormalGameContext(void) {
      soundOutput = &soundIO;
      controlPanelReader = &soundIO;
      controlPanelWriter = &soundIO;
   }

private:
   UWPSoundIO soundIO;
};


#endif
