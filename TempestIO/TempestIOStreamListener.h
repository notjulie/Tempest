
#ifndef TEMPESTIOSTREAMLISTENER_H
#define TEMPESTIOSTREAMLISTENER_H

class AbstractTempestIO;
class AbstractTempestStream;

class TempestIOStreamListener {
public:
   TempestIOStreamListener(AbstractTempestStream *stream, AbstractTempestIO *tempestIO);

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
   AbstractTempestStream *stream;
   AbstractTempestIO *tempestIO;

   State state;
   uint16_t vectorRAMAddress;
   uint8_t soundChannel;
};

#endif
