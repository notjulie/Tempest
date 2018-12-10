
#include <stdafx.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include "TempestException.h"

#include "PiSerialStream.h"


PiSerialStream::PiSerialStream(void)
{
   // clear
   fileStream = -1;
   terminated = false;

   writeBufferIn = 0;
   writeBufferOut = 0;
   writeThreadFailed = false;
   writeStatus = "Thread not started";
   writeCount = 0;

   readBufferIn = 0;
   readBufferOut = 0;
   readThreadFailed = false;
   readStatus = "Thread not started";
   readCount = 0;

   // open the serial device
   // this is the Disco's USB name
   fileStream = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);

   // this is the FTDI's USB name
   if (fileStream == -1)
   {
      fileStream = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY);
   }

   // else we use the built-in serial port, which will probably become
   // the permanent solution
   if (fileStream == -1)
   {
      fileStream = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY);
   }

   if (fileStream == -1)
      throw TempestException("Failure connecting to Disco");

   // set options
   termios options;
   tcgetattr(fileStream, &options);
   options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
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
   if (readThreadFailed)
      throw TempestException(readThreadError);

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
   if (readThreadFailed)
      throw TempestException(readThreadError);

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

bool PiSerialStream::Write(uint8_t b)
{
   if (writeThreadFailed)
      return false;

   // figure out what our index will be after appending the byte
   int newBufferIn = writeBufferIn + 1;
   if (newBufferIn >= (int)sizeof(writeBuffer))
      newBufferIn = 0;
   if (newBufferIn == writeBufferOut)
      return false;

   // append
   writeBuffer[writeBufferIn] = b;
   writeBufferIn = newBufferIn;

   // set the event
   writeBufferEvent.notify_all();
   return true;
}


void PiSerialStream::WriteThread(void)
{
   try {
      for (;;)
      {
         // wait to be signalled
         writeStatus = "Waiting for something to write";
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
         writeStatus = "Writing";
         if (writeEnd != writeBufferOut)
         {
            ssize_t bytesWritten = write(fileStream, &writeBuffer[writeBufferOut], writeEnd - writeBufferOut);
            if (bytesWritten != writeEnd - writeBufferOut)
               throw TempestException("Serial write error");
            writeCount += bytesWritten;
         }

         // remove the data from the buffer
         writeStatus = "Removing written data from buffer";
         if (writeEnd >= (int)sizeof(writeBuffer))
            writeBufferOut = 0;
         else
            writeBufferOut = writeEnd;

         // set the event again if there's still data to write
         if (writeBufferIn != writeBufferOut)
            writeBufferEvent.notify_all();
      }
   }
   catch (TempestException &tx)
   {
      writeThreadFailed = true;
      writeThreadError = std::string("PiSerialStream write error: ") + tx.what();
   }
   catch (...)
   {
      writeThreadFailed = true;
      writeThreadError = "Unknown PiSerialStream write error";
   }

   if (writeThreadFailed)
      writeStatus = writeThreadError.c_str();
   else
      writeStatus = "Write thread exited";
}


void PiSerialStream::ReadThread(void)
{
   try {
      while (!terminated)
      {
         // read a byte
         readStatus = "Reading";
         uint8_t b;
         int bytesRead = read(fileStream, &b, 1);
         if (bytesRead != 1)
            break;
         readCount++;

         // figure out what our index will be after appending the byte
         readStatus = "Appending to the buffer";
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
   catch (TempestException &tx)
   {
      readThreadFailed = true;
      readThreadError = std::string("PiSerialStream read error: ") + tx.what();
   }
   catch (...)
   {
      readThreadFailed = true;
      readThreadError = "Unknown PiSerialStream read error";
   }

   if (readThreadFailed)
      readStatus = readThreadError.c_str();
   else
      readStatus = "Read thread exited";
}
