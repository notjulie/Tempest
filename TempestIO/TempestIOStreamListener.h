
#ifndef TEMPESTIOSTREAMLISTENER_H
#define TEMPESTIOSTREAMLISTENER_H

#include "PacketStream.h"
#include "SoundIOPacket.h"

class AbstractTempestSoundIO;
class AbstractTempestStream;

class TempestIOStreamListener {
public:
   TempestIOStreamListener(AbstractTempestStream *stream, AbstractTempestSoundIO *tempestIO);

   void Service(void);

private:
   PacketStream stream;
   AbstractTempestSoundIO *tempestIO;

   uint8_t encoder;
   uint8_t buttons;
   uint64_t lastPacketCpuTime;
   TempestInPacket lastResponsePacket;
   uint64_t lastResponsePacketTime;
};

#endif
