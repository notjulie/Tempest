
#include <stdafx.h>

#include "TempestCPU/TempestException.h"

#include "PiSerialStream.h"


PiSerialStream::PiSerialStream(void)
{
   // clear
   fileStream = -1;
   terminated = false;

   writeBufferIn = 0;
   writeBufferOut = 0;
   writeThreadFailed = false;

   readBufferIn = 0;
   readBufferOut = 0;
   readThreadFailed = false;

   // open the serial device
   fileStream = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);
   if (fileStream == -1)
      throw TempestException("Failure connecting to Disco");

   // set options
   termios options;
   tcgetattr(fileStream, &options);
   options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
   options.c_iflag = IGNPAR;
   options.c_oflag = 0;
   options.c_lflag = 0;
   tcflush(fileStream, TCIFLUSH);
   tcsetattr(fileStream, TCSANOW, &options);

   // start our write thread
   writeThread = new std::thread(
      [this]() { WriteThread(); }
      );

   // start our read thread
   readThread = new std::thread(
      [this]() { ReadThread(); }
      );
}

PiSerialStream::~PiSerialStream(void)
{
   // note that we are terminating
   terminated = true;

   // wait for threads to exit
   writeThread->join();
   readThread->join();

   delete writeThread, writeThread = NULL;
   delete readThread, readThread = NULL;

   // close
   if (fileStream != -1)
      close(fileStream), fileStream = -1;
}

int PiSerialStream::Peek(void)
{
   // never mind if there's nothing
   if (readBufferIn == readBufferOut)
      return -1;

   // else just return the next
   int newBufferOut = readBufferOut + 1;
   if (newBufferOut >= (int)sizeof(readBuffer))
      newBufferOut = 0;
   return readBuffer[readBufferOut];
}

int PiSerialStream::Read(void)
{
   // never mind if there's nothing
   if (readBufferIn == readBufferOut)
      return -1;

   // else just return the next
   int newBufferOut = readBufferOut + 1;
   if (newBufferOut >= (int)sizeof(readBuffer))
      newBufferOut = 0;
   int result = readBuffer[readBufferOut];
   readBufferOut = newBufferOut;
   return result;
}

void PiSerialStream::Write(uint8_t b)
{
   // figure out what our index will be after appending the byte
   int newBufferIn = writeBufferIn + 1;
   if (newBufferIn >= (int)sizeof(writeBuffer))
      newBufferIn = 0;
   if (newBufferIn == writeBufferOut)
      throw TempestException("Serial write buffer full");

   // append
   writeBuffer[writeBufferIn] = b;
   writeBufferIn = newBufferIn;

   // set the event
   writeBufferEvent.notify_all();
}


void PiSerialStream::WriteThread(void)
{
   for (;;)
   {
      // wait to be signalled
      {
         std::unique_lock<std::mutex> lock(writeBufferMutex);
         writeBufferEvent.wait_until(
            lock,
            std::chrono::system_clock::now() + std::chrono::seconds(1)
            );
      }

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
      if (writeEnd >= (int)sizeof(writeBuffer))
         writeBufferOut = 0;
      else
         writeBufferOut = writeEnd;

      // set the event again if there's still data to write
      if (writeBufferIn != writeBufferOut)
         writeBufferEvent.notify_all();
   }
}


void PiSerialStream::ReadThread(void)
{
   while (!terminated)
   {
      // read a byte
      uint8_t b;
      int bytesRead = read(fileStream, &b, 1);
      if (bytesRead != 1)
         break;

      // figure out what our index will be after appending the byte
      int newBufferIn = readBufferIn + 1;
      if (newBufferIn >= (int)sizeof(readBuffer))
         newBufferIn = 0;
      if (newBufferIn == readBufferOut)
         throw TempestException("Serial read buffer full");

      // append
      readBuffer[readBufferIn] = b;
      readBufferIn = newBufferIn;
   }
}
