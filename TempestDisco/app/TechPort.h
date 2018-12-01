
#ifndef TECHPORT_H
#define TECHPORT_H

#include "TempestMemoryStream.h"

class TechPort {
public:
	TechPort(void);
	void Initialize(void);
	void TransmitNext(void);

	void WriteString(const char *s);

private:
	SimpleMemoryStream stream;
};

extern TechPort Tech;

#endif
