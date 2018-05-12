
#ifndef TEMPESTIOSTREAMLISTENER_H
#define TEMPESTIOSTREAMLISTENER_H

#include "PacketStream.h"
#include "SoundIOPacket.h"

class AbstractTempestSoundOutput;
class AbstractTempestStream;

class TempestIOStreamListener {
public:
   TempestIOStreamListener(
      AbstractTempestStream *stream,
      AbstractTempestSoundOutput *tempestIO,
      AbstractArcadeGameControlPanel *controlPanel
   );

   void Service(void);

private:
   PacketStream stream;
   AbstractTempestSoundOutput *soundOutput = nullptr;
   AbstractArcadeGameControlPanel *controlPanel = nullptr;

   uint8_t encoder = 0;
   uint8_t buttons = 0;
   uint64_t lastPacketCpuTime = 0;
   TempestInPacket lastResponsePacket;
   uint64_t lastResponsePacketTime = 0;
};

#endif
