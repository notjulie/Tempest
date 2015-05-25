
#include "stdafx.h"

#include "../TempestCPU/TempestException.h"

#include "TempestMemoryStream.h"

TempestMemoryStream::TempestMemoryStream(void)
{
	bufferIn = 0;
	bufferOut = 0;
}

void TempestMemoryStream::Write(uint8_t b)
{
	// figure out what our index will be after appending the byte
	int newBufferIn = bufferIn + 1;
	if (newBufferIn >= sizeof(buffer))
		newBufferIn = 0;
	if (newBufferIn == bufferOut)
		throw TempestException("TempestMemoryStream::Write: buffer full");

	buffer[bufferIn] = b;
	bufferIn = newBufferIn;
}
