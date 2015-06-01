
#ifndef TEMPESTIOSTREAMLISTENER_H
#define TEMPESTIOSTREAMLISTENER_H

class AbstractTempestIO;
class AbstractTempestStream;

class TempestIOStreamListener {
public:
   TempestIOStreamListener(AbstractTempestStream *stream, AbstractTempestIO *tempestIO);

private:
   AbstractTempestStream *stream;
   AbstractTempestIO *tempestIO;
};

#endif
