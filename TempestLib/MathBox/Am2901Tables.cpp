
#include "stdafx.h"
#include "Am2901Tables.h"


Am2901Tables::Am2901Tables(void)
{
	for (uint8_t r = 0; r < 16; ++r)
	{
		for (uint8_t s = 0; s < 16; ++s)
		{
			c3Table[(r << 5) + (s << 1)] = CalculateC3(r, s, false);
			c3Table[(r << 5) + (s << 1) + 1] = CalculateC3(r, s, true);
		}
	}
}


bool Am2901Tables::CalculateC3(Nybble R, Nybble S, bool CarryIn)
{
	Nybble P = R | S;
	Nybble G = R & S;
	return
		G[2] ||
		(P[2] && G[1]) ||
		(P[2] && P[1] && G[0]) ||
		(P[2] && P[1] && P[0] && CarryIn);
}

