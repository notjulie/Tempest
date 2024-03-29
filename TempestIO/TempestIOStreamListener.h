
#ifndef TEMPESTIOSTREAMLISTENER_H
#define TEMPESTIOSTREAMLISTENER_H

#include "PacketStream.h"
#include "SoundIOPacket.h"

class AbstractSoundGenerator;
class AbstractTempestStream;

class TempestIOStreamListener {
public:
   TempestIOStreamListener(
      AbstractTempestStream *stream,
      AbstractSoundGenerator *tempestIO,
      AbstractArcadeGameControlPanelReader *controlPanel
   );

   void Service(void);

private:
   PacketStream stream;
   AbstractSoundGenerator *soundOutput;
   AbstractArcadeGameControlPanelReader *controlPanel;

   uint8_t encoder;
   uint8_t buttons;
   uint64_t lastPacketCpuTime;
   TempestInPacket lastResponsePacket;
   uint64_t lastResponsePacketTime;
};

#endif
