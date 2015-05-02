
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

Tristate Tristate::operator!(void) const
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
	// if either is true then the result is true
	if (!IsUnknown() && Value())
		return true;
	if (!t2.IsUnknown() && t2.Value())
		return true;

	// else we can fall back on the base operator
	return operator|(t2);
}


