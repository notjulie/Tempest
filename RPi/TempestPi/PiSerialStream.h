
#ifndef PISERIALSTREAM_H
#define PISERIALSTREAM_H

#include "TempestIO/AbstractTempestStream.h"

class PiSerialStream : public AbstractTempestStream
{
public:
   PiSerialStream(void);
   virtual ~PiSerialStream(void);

   virtual int  Read(void);
   virtual void Write(uint8_t b);

private:
   void WriteThread(void);

private:
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
};

#endif
