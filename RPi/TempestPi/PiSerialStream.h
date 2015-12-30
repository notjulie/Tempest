
#ifndef PISERIALSTREAM_H
#define PISERIALSTREAM_H

#include "TempestIO/AbstractTempestStream.h"

class PiSerialStream : public AbstractTempestStream
{
public:
   PiSerialStream(void);
   virtual ~PiSerialStream(void);

   virtual int  Peek(void);
   virtual int  Read(void);
   virtual void Write(uint8_t b);

private:
   void ReadThread(void);
   void WriteThread(void);

private:
   static void *ReadThreadEntry(void *pThis);
   static void *WriteThreadEntry(void *pThis);

private:
   int fileStream;
   bool terminated;

   uint8_t writeBuffer[20000];
   int writeBufferIn;
   int writeBufferOut;
   pthread_mutex_t writeBufferMutex;
   pthread_cond_t writeBufferEvent;
   pthread_t writeThread;
   bool writeThreadFailed;
   std::string writeThreadError;

   uint8_t readBuffer[20000];
   int readBufferIn;
   int readBufferOut;
   pthread_t readThread;
   bool readThreadFailed;
   std::string readThreadError;
};

#endif
