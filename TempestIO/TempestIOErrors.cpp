
#include "stdafx.h"
#include "../TempestCPU/TempestException.h"
#include "TempestMemoryStream.h"
#include "WaveStreamer.h"

// I allow classes to define error reporting functions that are not
// implemented so that the specific implementation can decide whether
// it wants to handle them as excepstions or by some other means.
// This contains the implementation for the TempestIO library.

void SimpleMemoryStream::ReportBufferOverflow(void)
{
   throw TempestException("SimpleMemoryStream: buffer overflow");
}

