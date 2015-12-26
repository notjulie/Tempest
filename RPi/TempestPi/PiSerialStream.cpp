
#include <stdafx.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#include "TempestCPU/TempestException.h"

#include "PiSerialStream.h"


PiSerialStream::PiSerialStream(void)
{
   fileStream = -1;

   fileStream = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_NDELAY);
   if (fileStream == -1)
      throw TempestException("Failure connecting to Disco");
}

PiSerialStream::~PiSerialStream(void)
{
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
   ssize_t bytesWritten = write(fileStream, &b, 1);
   if (bytesWritten != 1)
      throw TempestException("Serial write error");
}
