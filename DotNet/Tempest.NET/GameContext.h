
#ifndef GAMECONTEXT_H
#define GAMECONTEXT_H

#include "TempestMemoryStream.h"
#include "Win32ComPortStream.h"
#include "Win32TempestIO.h"

class TempestIOStreamListener;
class TempestIOStreamProxy;

/// <summary>
/// Abstract class for classes that want to provide the external connections
/// from the game to the world.
/// </summary>
class GameContext {
public:
   GameContext(void) {}
   virtual ~GameContext(void) {}

   AbstractTempestSoundOutput *GetSoundOutput(void) const { return soundOutput; }
   AbstractArcadeGameControlPanelReader *GetControlPanelReader(void) const { return controlPanelReader; }
   AbstractArcadeGameControlPanelWriter *GetControlPanelWriter(void) const { return controlPanelWriter; }

protected:
   AbstractTempestSoundOutput * soundOutput = nullptr;
   AbstractArcadeGameControlPanelReader *controlPanelReader = nullptr;
   AbstractArcadeGameControlPanelWriter *controlPanelWriter = nullptr;
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
   Win32TempestSoundIO soundIO;
};

/// <summary>
/// GameContext implementation for games that want to connect to the Windows hardware
/// via a serialized stream.  This is useful for testing the streaming that is used
/// to connect to the Tempest hardware via serial connection, while still running
/// everything on Windows.
/// </summary>
class SerializedGameContext : public GameContext {
public:
   SerializedGameContext(void);
   virtual ~SerializedGameContext(void);

private:
   void PollListener(void);

private:
   bool terminated = false;
   TempestMemoryStream stream;
   Win32TempestSoundIO tempestSoundIO;
   TempestIOStreamListener *listener = nullptr;
   TempestIOStreamProxy *proxy = nullptr;

   // this is an std::thread, but managed C++ will choke on this header if it includes
   // <thread>, so I just cast to void
   void *pollThread = nullptr;
};


/// <summary>
/// GameContext implementation for games that want to the actual Tempest hardware via
/// a COM port.
/// </summary>
class COMPortGameContext : public GameContext {
public:
   COMPortGameContext(const std::string &portName);
   virtual ~COMPortGameContext(void);

private:
   Win32ComPortStream comPortStream;
   SimpleArcadeGameControlPanel placebo;
   TempestIOStreamProxy *proxy = nullptr;
};


/// <summary>
/// GameContext implementation for games that want to connect to the Windows hardware
/// via a serial loopback stream.
/// </summary>
class LoopbackGameContext : public GameContext {
public:
   LoopbackGameContext(const std::string &port1, const std::string &port2);
   virtual ~LoopbackGameContext(void);

private:
   void PollListener(void);

private:
   bool terminated = false;
   Win32TempestSoundIO tempestSoundIO;
   TempestIOStreamListener *listener = nullptr;
   TempestIOStreamProxy *proxy = nullptr;
   Win32ComPortStream comPort1Stream;
   Win32ComPortStream comPort2Stream;

   // this is an std::thread, but managed C++ will choke on this header if it includes
   // <thread>, so I just cast to void
   void *pollThread = nullptr;
};


#endif
