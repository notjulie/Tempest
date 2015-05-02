
#include "stdafx.h"
#include "MathBoxException.h"
#include "Tristate.h"

Tristate Tristate::Unknown;

Tristate::Tristate(void)
{
	isKnown = false;
}

Tristate::Tristate(bool value)
{
	isKnown = true;
	this->value = value;
}

bool Tristate::IsUnknown(void) const
{
	return !isKnown;
}

Tristate Tristate::operator!(void)
{
	if (isKnown)
		return !value;
	else
		return Unknown;
}

Tristate Tristate::operator && (const Tristate &t2) const
{
	// if either is false the result is false
	if (isKnown && !value)
		return false;
	if (t2.isKnown && !t2.value)
		return false;

	// else if either is not known the result is not known
	if (!isKnown || !t2.isKnown)
		return Unknown;

	// else its just an and
	return value && t2.value;
}

Tristate Tristate::operator ^ (const Tristate &t2) const
{
	// if either is not known the result is not known
	if (!isKnown || !t2.isKnown)
		return Unknown;

	// else its just an xor
	return value ^ t2.value;
}

Tristate::operator bool(void) const
{
	if (!isKnown)
		throw MathBoxException("Can't convert unknown Tristate value to bool");
	return value;
}


