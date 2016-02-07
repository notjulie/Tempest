
#ifndef TEMPESTIOSTREAMLISTENER_H
#define TEMPESTIOSTREAMLISTENER_H

#include "PacketStream.h"

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
};

#endif
