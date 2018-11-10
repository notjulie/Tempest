
#include "TempestIO.Headers.h"
#include "TempestMemoryStream.h"

SimpleMemoryStream::SimpleMemoryStream(void)
{
	bufferIn = 0;
	bufferOut = 0;
}

bool SimpleMemoryStream::Write(uint8_t b)
{
	// figure out what our index will be after appending the byte
	int newBufferIn = bufferIn + 1;
	if (newBufferIn >= (int)sizeof(buffer))
		newBufferIn = 0;
	if (newBufferIn == bufferOut)
		return false;

	buffer[bufferIn] = b;
	bufferIn = newBufferIn;
	return true;
}

int SimpleMemoryStream::Peek(void)
{
   // never mind if there's nothing
   if (bufferIn == bufferOut)
      return -1;

   // else just return the next
   return buffer[bufferOut];
}


int SimpleMemoryStream::Read(void)
{
   // never mind if there's nothing
   if (bufferIn == bufferOut)
      return -1;

   // else just return the next
   int newBufferOut = bufferOut + 1;
   if (newBufferOut >= (int)sizeof(buffer))
      newBufferOut = 0;
   int result = buffer[bufferOut];
   bufferOut = newBufferOut;
   return result;
}


