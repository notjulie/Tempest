
#ifndef TRISTATE_H
#define TRISTATE_H

#include "Nullable.h"

class Tristate : public Nullable<bool>
{
public:
	Tristate(void);
	Tristate(bool value);
	Tristate(const Nullable<bool> value);

	Tristate operator!(void);
	Tristate operator&&(const Tristate &t2) const;
};

#endif
