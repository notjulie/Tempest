
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
      VECTOR_RAM_VALUE
   };

private:
   AbstractTempestStream *stream;
   AbstractTempestIO *tempestIO;

   State state;
   uint16_t vectorRAMAddress;
};

#endif
