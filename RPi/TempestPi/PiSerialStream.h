
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
   int fileStream;
   bool terminated;

   std::thread *writeThread;
   uint8_t writeBuffer[20000];
   int writeBufferIn;
   int writeBufferOut;
   std::mutex writeBufferMutex;
   std::condition_variable writeBufferEvent;
   bool writeThreadFailed;
   std::string writeThreadError;

   std::thread *readThread;
   uint8_t readBuffer[20000];
   int readBufferIn;
   int readBufferOut;
   bool readThreadFailed;
   std::string readThreadError;
};

#endif
