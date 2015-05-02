
#include "stdafx.h"
#include "MathBoxException.h"
#include "Tristate.h"


Tristate::Tristate(void)
{
}

Tristate::Tristate(bool value)
	: Nullable<bool>(value)
{
}

Tristate::Tristate(const Nullable<bool> value)
	: Nullable<bool>(value)
{
}

Tristate Tristate::operator!(void)
{
	if (IsUnknown())
		return Unknown;
	else
		return !Value();
}

Tristate Tristate::operator && (const Tristate &t2) const
{
	// this is the same as the single & operator for bools
	return operator&(t2);
}


Tristate Tristate::operator || (const Tristate &t2) const
{
	// for now this is the same as the single & operator for bools;
	// at the moment this doesn't take into account the fact that
	// 1 | x = 1
	return operator|(t2);
}


