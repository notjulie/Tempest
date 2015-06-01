
#include "stdafx.h"
#include "TempestIOStreamListener.h"


TempestIOStreamListener::TempestIOStreamListener(AbstractTempestStream *stream, AbstractTempestIO *tempestIO)
{
   // copy parameters
   this->stream = stream;
   this->tempestIO = tempestIO;
}
