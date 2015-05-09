
#ifndef ABSTRACTTEMPESTWAVESTREAM_H
#define ABSTRACTTEMPESTWAVESTREAM_H

class AbstractTempestWaveStream
{
public:
	virtual ~AbstractTempestWaveStream(void) {}

	virtual void ReadWaveData(int16_t *buffer, int count) = 0;
};

#endif
