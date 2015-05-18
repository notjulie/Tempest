
#ifndef AM2901TABLES_H
#define AM2901TABLES_H

#include "Nybble.h"

class Am2901Tables
{
public:
	Am2901Tables(void);

	inline bool GetC3(uint8_t R, uint8_t S, bool CarryIn) const { return c3Table[(R << 5) + (S << 1) + (CarryIn ? 1 : 0)]; }

public:
	// this one is too easy to bother with a table
	static inline bool GetC4(uint8_t R, uint8_t S, bool CarryIn) { return R + S + (CarryIn ? 1 : 0) > 15; }

private:
	static bool CalculateC3(Nybble R, Nybble S, bool CarryIn);

private:
	bool  c3Table[512];
};

#endif
