
#ifndef AM2901_H
#define AM2901_H

#include <stdint.h>

class Am2901
{
public:
	uint8_t A;
	uint8_t B;
	uint16_t I;

	bool CarryIn;
	bool CarryOut;

public:
	void Update(void);
};

#endif
