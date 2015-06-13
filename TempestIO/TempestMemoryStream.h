
#ifndef TEMPESTMEMORYSTREAM_H
#define TEMPESTMEMORYSTREAM_H

#include "AbstractTempestStream.h"
#include "TempestStreamProtocol.h"


class SimpleMemoryStream
{
public:
   SimpleMemoryStream(void);

   int  Read(void);
   void Write(uint8_t b);

private:
   // implementation dependent... normally one would throw an exception
   // here but for embedded exceptions may not be enabled
   static void ReportBufferOverflow(void);

private:
	uint8_t buffer[20000];
	int bufferIn;
	int bufferOut;
};

class FullDuplexStream : public AbstractTempestStream
{
public:
   FullDuplexStream(SimpleMemoryStream *readStream, SimpleMemoryStream *writeStream) {
      this->readStream = readStream;
      this->writeStream = writeStream;
   }

   virtual int  Read(void) { return readStream->Read(); }
   virtual void Write(uint8_t b) { writeStream->Write(b); }

private:
   SimpleMemoryStream *readStream;
   SimpleMemoryStream *writeStream;
};

class TempestMemoryStream
{
public:
   TempestMemoryStream(void)
      :
      leftSide(&rightToLeft, &leftToRight),
      rightSide(&leftToRight, &rightToLeft)
         {}
   AbstractTempestStream *GetLeftSide(void) { return &leftSide; }
   AbstractTempestStream *GetRightSide(void) { return &rightSide; }

private:
   SimpleMemoryStream leftToRight;
   SimpleMemoryStream rightToLeft;
   FullDuplexStream leftSide;
   FullDuplexStream rightSide;
};

#endif
