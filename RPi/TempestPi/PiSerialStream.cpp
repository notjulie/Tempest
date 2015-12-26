
#include <stdafx.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#include "TempestCPU/TempestException.h"

#include "PiSerialStream.h"


PiSerialStream::PiSerialStream(void)
{
   // clear
   fileStream = -1;
   writeBufferIn = 0;
   writeBufferOut = 0;
   writeBufferMutex = PTHREAD_MUTEX_INITIALIZER;
   writeBufferEvent = PTHREAD_COND_INITIALIZER;
   terminated = false;
   writeThreadFailed = false;

   // open the serial device
   fileStream = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);
   if (fileStream == -1)
      throw TempestException("Failure connecting to Disco");

   // start our write thread
   int result = pthread_create(
      &writeThread,
      NULL,
      &WriteThreadEntry,
      this
      );
   if (result != 0)
      throw TempestException("Error creating serial write thread");
   pthread_setname_np(writeThread, "SerialWrite");
}

PiSerialStream::~PiSerialStream(void)
{
   // note that we are terminating
   terminated = true;

   // wait for threads to exit
   pthread_join(writeThread, NULL);

   // close
   pthread_cond_destroy(&writeBufferEvent);
   if (fileStream != -1)
      close(fileStream), fileStream = -1;
}

int PiSerialStream::Read(void)
{
   if (fileStream == -1)
      return -1;

   uint8_t b;
   int bytesRead = read(fileStream, &b, 1);
   if (bytesRead == 1)
      return b;
   else
      return -1;
}

void PiSerialStream::Write(uint8_t b)
{
   // figure out what our index will be after appending the byte
   int newBufferIn = writeBufferIn + 1;
   if (newBufferIn >= sizeof(writeBuffer))
      newBufferIn = 0;
   if (newBufferIn == writeBufferOut)
      throw TempestException("Serial write buffer full");

   // append
   writeBuffer[writeBufferIn] = b;
   writeBufferIn = newBufferIn;

   // set the event
   pthread_mutex_lock(&writeBufferMutex);
   pthread_cond_signal(&writeBufferEvent);
   pthread_mutex_unlock(&writeBufferMutex);
}


void *PiSerialStream::WriteThreadEntry(void *pThis)
{
   PiSerialStream *stream = (PiSerialStream*)pThis;

   try
   {
      stream->WriteThread();
      return NULL;
   }
   catch (TempestException &te)
   {
      stream->writeThreadFailed = true;
      stream->writeThreadError = te.what();
   }
   catch (...)
   {
      stream->writeThreadFailed = true;
      stream->writeThreadError = "Unknown exception";
   }
}

void PiSerialStream::WriteThread(void)
{
   for (;;)
   {
      // wait to be signalled
      timespec waitUntil;
      clock_gettime(CLOCK_REALTIME, &waitUntil);
      waitUntil.tv_nsec += 1000000;

      pthread_mutex_lock(&writeBufferMutex);
      pthread_cond_timedwait(&writeBufferEvent, &writeBufferMutex, &waitUntil);
      pthread_mutex_unlock(&writeBufferMutex);

      // check for termination
      if (terminated)
         break;

      // write everything between here and the end of the data or the end of
      // the buffer, whichever comes first
      int writeEnd = writeBufferIn;
      if (writeEnd < writeBufferOut)
         writeEnd = sizeof(writeBuffer);

      // write
      if (writeEnd != writeBufferOut)
      {
         ssize_t bytesWritten = write(fileStream, &writeBuffer[writeBufferOut], writeEnd - writeBufferOut);
         if (bytesWritten != writeEnd - writeBufferOut)
            throw TempestException("Serial write error");
      }

      // remove the data from the buffer
      if (writeEnd >= sizeof(writeBuffer))
         writeBufferOut = 0;
      else
         writeBufferOut = writeEnd;

      // set the event again if there's still data to write
      if (writeBufferIn != writeBufferOut)
      {
         pthread_mutex_lock(&writeBufferMutex);
         pthread_cond_signal(&writeBufferEvent);
         pthread_mutex_unlock(&writeBufferMutex);
      }
   }
}
