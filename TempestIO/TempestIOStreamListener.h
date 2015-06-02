
#ifndef TEMPESTIOSTREAMLISTENER_H
#define TEMPESTIOSTREAMLISTENER_H

class AbstractTempestIO;
class AbstractTempestInStream;

class TempestIOStreamListener {
public:
   TempestIOStreamListener(AbstractTempestInStream *stream, AbstractTempestIO *tempestIO);

   void Service(void);

private:
   enum State {
      IDLE,
      VECTOR_RAM_ADDRESS_LOW,
      VECTOR_RAM_VALUE,
      SOUND_VOLUME,
      SOUND_WAVE,
      SOUND_FREQUENCY
   };

private:
   AbstractTempestInStream *stream;
   AbstractTempestIO *tempestIO;

   State state;
   uint16_t vectorRAMAddress;
   uint8_t soundChannel;
};

#endif
