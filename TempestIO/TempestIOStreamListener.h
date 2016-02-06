
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
   enum State {
      IDLE,
      BUTTON_LEDS,
      SOUND_VOLUME,
      SOUND_WAVE,
      SOUND_FREQUENCY
   };

private:
   PacketStream stream;
   AbstractTempestSoundIO *tempestIO;

   State state;
   uint8_t soundChannel;
   uint8_t encoder;
   uint8_t buttons;
};

#endif
