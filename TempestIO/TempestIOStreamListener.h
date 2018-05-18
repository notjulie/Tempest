
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
      AbstractArcadeGameControlPanelReader *controlPanel
   );

   void Service(void);

private:
   PacketStream stream;
   AbstractTempestSoundOutput *soundOutput;
   AbstractArcadeGameControlPanelReader *controlPanel;

   uint8_t encoder;
   uint8_t buttons;
   uint64_t lastPacketCpuTime;
   TempestInPacket lastResponsePacket;
   uint64_t lastResponsePacketTime;
};

#endif
