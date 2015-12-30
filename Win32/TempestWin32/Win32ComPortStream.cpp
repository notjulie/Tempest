
#include "stdafx.h"

#include "TempestCPU/TempestException.h"

#include "Win32ComPortStream.h"


Win32ComPortStream::Win32ComPortStream(const char *portName)
{
   // clear
   readBufferIn = 0;
   readBufferOut = 0;
   writeBufferIn = 0;
   writeBufferOut = 0;
   memset(&readOverlapped, 0, sizeof(readOverlapped));
   memset(&writeOverlapped, 0, sizeof(writeOverlapped));

   // create a device name from the com port name
   std::string deviceName = "\\\\.\\";
   deviceName += portName;

   // open it
   file = CreateFile(
      deviceName.c_str(),
      GENERIC_READ | GENERIC_WRITE,
      0,
      NULL,
      OPEN_EXISTING,
      FILE_FLAG_OVERLAPPED,
      NULL
      );
   if (file == INVALID_HANDLE_VALUE)
      throw TempestException("Win32ComPortStream::Win32ComPortStream: CreateFile failed");

   // set our comm timeouts
   COMMTIMEOUTS   commTimeouts;

   // this says that we are not using timeouts for write operations
   commTimeouts.WriteTotalTimeoutConstant = 0;
   commTimeouts.WriteTotalTimeoutMultiplier = 0;

   // this says, according to the doc, that
   //   - if there are bytes in the buffer read returns immediately
   //   - if there are no bytes it waits for a byte and returns immediately
   //   - if no bytes arrive before ReadTotalTimeoutConstant it times out
   commTimeouts.ReadIntervalTimeout = MAXDWORD;
   commTimeouts.ReadTotalTimeoutMultiplier = MAXDWORD;
   commTimeouts.ReadTotalTimeoutConstant = 100;
   SetCommTimeouts(file, &commTimeouts);

   // create our events
   writeBufferEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
   terminateEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
   readOverlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
   writeOverlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

   // start our read & write threads
   readThread = CreateThread(
      NULL,
      0,
      (LPTHREAD_START_ROUTINE)ReadThreadEntry,
      this,
      0,
      &readThreadID);
   writeThread = CreateThread(
      NULL,
      0,
      (LPTHREAD_START_ROUTINE)WriteThreadEntry,
      this,
      0,
      &writeThreadID);
}

Win32ComPortStream::~Win32ComPortStream(void)
{
   // note that we are terminating
   SetEvent(terminateEvent);

   // wait for the threads to exit
   WaitForSingleObject(readThread, INFINITE);
   WaitForSingleObject(writeThread, INFINITE);

   // close the file
   CloseHandle(file);

   // close the events
   CloseHandle(readOverlapped.hEvent);
   CloseHandle(writeOverlapped.hEvent);
   CloseHandle(writeBufferEvent);
}

void Win32ComPortStream::Write(uint8_t b)
{
   // figure out what our index will be after appending the byte
   int newBufferIn = writeBufferIn + 1;
   if (newBufferIn >= sizeof(writeBuffer))
      newBufferIn = 0;
   if (newBufferIn == writeBufferOut)
      throw TempestException("Win32ComPortStream::Write: buffer full");

   writeBuffer[writeBufferIn] = b;
   writeBufferIn = newBufferIn;

   // set the event
   SetEvent(writeBufferEvent);
}

int Win32ComPortStream::Peek(void)
{
   // bail if our read thread hit an error
   if (readThreadError.size() > 0)
      throw TempestException(readThreadError);

   // never mind if there's nothing
   if (readBufferIn == readBufferOut)
      return -1;

   // else just return the next
   return readBuffer[readBufferOut];
}

int Win32ComPortStream::Read(void)
{
   // bail if our read thread hit an error
   if (readThreadError.size() > 0)
      throw TempestException(readThreadError);

   // never mind if there's nothing
   if (readBufferIn == readBufferOut)
      return -1;

   // else just return the next
   int newBufferOut = readBufferOut + 1;
   if (newBufferOut >= sizeof(readBuffer))
      newBufferOut = 0;
   int result = readBuffer[readBufferOut];
   readBufferOut = newBufferOut;
   return result;
}


void Win32ComPortStream::ReadThread(void)
{
   while (WaitForSingleObject(terminateEvent, 0) == WAIT_TIMEOUT)
   {
      // clear our event
      ResetEvent(readOverlapped.hEvent);

      // read
      uint8_t buffer[100];
      DWORD bytesRead;
      BOOL readResult = ReadFile(
         file,
         buffer,
         sizeof(buffer),
         &bytesRead,
         &readOverlapped
         );

      // if we get an error see if it was actually an error or if we just have to wait
      if (!readResult)
      {
         DWORD error = GetLastError();
         if (error != ERROR_IO_PENDING)
            return;

         // else wait
         HANDLE waitHandles[] = { terminateEvent, readOverlapped.hEvent };
         DWORD waitResult = WaitForMultipleObjects(2, waitHandles, FALSE, INFINITE);
         if (waitResult == WAIT_OBJECT_0)
         {
            CancelIo(file);
            break;
         }

         // get the result
         if (!GetOverlappedResult(file, &readOverlapped, &bytesRead, FALSE))
            return;
      }

      // process the data
      for (unsigned i = 0; i < bytesRead; ++i)
      {
         // figure out what our index will be after appending the byte
         int newBufferIn = readBufferIn + 1;
         if (newBufferIn >= sizeof(readBuffer))
            newBufferIn = 0;
         if (newBufferIn == readBufferOut)
         {
            readThreadError = "Win32ComPortStream::ReadThread: buffer full";
            return;
         }

         readBuffer[readBufferIn] = buffer[i];
         readBufferIn = newBufferIn;
      }
   }
}


void Win32ComPortStream::WriteThread(void)
{
   for (;;)
   {
      // wait to be signaled that there's something to write
      {
         HANDLE waitHandles[] = { terminateEvent, writeBufferEvent };
         DWORD waitResult = WaitForMultipleObjects(2, waitHandles, FALSE, INFINITE);
         if (waitResult == WAIT_OBJECT_0)
            break;
      }

      // clear our event
      ResetEvent(writeOverlapped.hEvent);

      // write everything between here and the end of the data or the end of the buffer,
      // whichever comes first
      int   writeEnd = writeBufferIn;
      if (writeEnd < writeBufferOut)
         writeEnd = sizeof(writeBuffer);
      BOOL writeResult = WriteFile(
         file,
         &writeBuffer[writeBufferOut],
         writeEnd - writeBufferOut,
         NULL,
         &writeOverlapped
         );

      // if the call fails see if it was an error or if it's just not finished yet
      if (!writeResult)
      {
         // see if it was actually an error
         DWORD error = GetLastError();
         if (error != ERROR_IO_PENDING)
            return;

         // else wait
         HANDLE waitHandles[] = { terminateEvent, writeOverlapped.hEvent };
         DWORD waitResult = WaitForMultipleObjects(2, waitHandles, FALSE, INFINITE);
         if (waitResult == WAIT_OBJECT_0)
         {
            CancelIo(file);
            break;
         }

         // get the result
         DWORD bytesWritten;
         if (!GetOverlappedResult(file, &writeOverlapped, &bytesWritten, FALSE))
            return;
      }

      // remove the data from the buffer
      if (writeEnd >= sizeof(writeBuffer))
         writeBufferOut = 0;
      else
         writeBufferOut = writeEnd;

      // set the event if there's still data to write
      if (writeBufferIn != writeBufferOut)
         SetEvent(writeBufferEvent);
   }
}
