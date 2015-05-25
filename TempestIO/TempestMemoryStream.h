
#ifndef TEMPESTMEMORYSTREAM_H
#define TEMPESTMEMORYSTREAM_H

#include "AbstractTempestStream.h"

class TempestMemoryStream : public AbstractTempestStream
{
public:
	TempestMemoryStream(void);

	virtual void Write(uint8_t b);

private:
	uint8_t buffer[10000];
	int bufferIn;
	int bufferOut;
};


#endif
