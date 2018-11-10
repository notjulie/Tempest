
#include "TempestIO.Headers.h"
#include "AbstractTempestStream.h"

void AbstractTempestStream::WriteString(const char *s)
{
	while (*s)
		Write((uint8_t)(*s++));
}

