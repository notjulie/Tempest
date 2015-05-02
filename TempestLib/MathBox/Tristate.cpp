
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
		return !(bool)*this;
}

Tristate Tristate::operator && (const Tristate &t2) const
{
	// this is the same as the single & operator for bools
	return operator&(t2);
}



