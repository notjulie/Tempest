
#ifndef WIN32COMPORTSTREAM_H
#define WIN32COMPORTSTREAM_H

#include "TempestIO/AbstractTempestStream.h"


class Win32ComPortStream : public AbstractTempestStream
{
public:
   Win32ComPortStream(const char *portName);
   virtual ~Win32ComPortStream(void);

   virtual int  Peek(void);
   virtual int  Read(void);
   virtual void Write(uint8_t b);

private:
   static long ReadThreadEntry(Win32ComPortStream *pThis) { pThis->ReadThread(); return 0; }
   static long WriteThreadEntry(Win32ComPortStream *pThis) { pThis->WriteThread(); return 0; }

private:
   void  ReadThread(void);
   void  WriteThread(void);

private:
   HANDLE   file;
   HANDLE   readThread;
   HANDLE   writeThread;
   DWORD    readThreadID;
   DWORD    writeThreadID;

   HANDLE   writeBufferEvent;
   HANDLE   terminateEvent;
   OVERLAPPED  readOverlapped;
   OVERLAPPED  writeOverlapped;

   uint8_t  readBuffer[10000];
   int      readBufferIn;
   int      readBufferOut;
   uint8_t  writeBuffer[20000];
   int      writeBufferIn;
   int      writeBufferOut;

   std::string readThreadError;
};

#endif
