// ====================================================================
// Tempest emulation project
//    Author: Randy Rasmussen
//    Copyright: none... do what you will
//    Warranties: none... do what you will at your own risk
//
// File summary:
//    Template class for classes that support data whose values
//    can be unknown
// ====================================================================


#ifndef NULLABLE_H
#define NULLABLE_H

#include "MathBoxException.h"
#include "Nybble.h"

// =======================================================================
// =======================================================================
// =============== Nullable template class definition ====================
// =======================================================================
// =======================================================================
template<class T> class Nullable
{
public:
	Nullable(void) { isKnown = false; }
	Nullable(T _value) { isKnown = true; value = _value; }

	bool IsUnknown(void) const { return !isKnown; }

	Nullable &operator=(const T &_value)
	{
		*this = Nullable(_value);
		return *this;
	}

	Nullable<bool> operator!(void) const
	{
		if (!isKnown)
			return Unknown;
		else
			return (value == (T)0);
	}

	Nullable<bool> operator&&(const Nullable &) const
	{
		// by default this is not implemented; certain classes can have
		// specific implementations, such as Nullable<bool>
		throw MathBoxException("Nullable::operator&& not implemented");
	}

	Nullable<bool> operator||(const Nullable &) const
	{
		// by default this is not implemented; certain classes can have
		// specific implementations, such as Nullable<bool>
		throw MathBoxException("Nullable::operator|| not implemented");
	}

	Nullable<bool> operator!=(const Nullable &_other) const
	{
		// if either is unknown we don't know if they are unequal
		if (!isKnown || _other.IsUnknown())
			return Tristate::Unknown;
		else
			return value != _other.Value();
	}

	Nullable<bool> operator==(const Nullable &_other) const
	{
		return !operator!=(_other);
	}

	Nullable<bool> operator[](int i) const
	{
		if (!isKnown)
			return Tristate::Unknown;
		else
			return (value & (1 << i)) != 0;
	}

	Nullable operator~(void) const
	{
		if (!isKnown)
			return Unknown;
		else
			return (T)~value;
	}

	Nullable operator|(const Nullable &t2) const
	{
		// if either value is the opposite of zero then that's what our
		// result is... OR with ones == ones
		Nullable identity = ~Nullable();
		Nullable<bool> isIdentity = (*this == identity) || (t2 == identity);
		if (!isIdentity.IsUnknown() && isIdentity.Value())
			return identity;

		// else if either is not known the result is not known
		if (!isKnown || !t2.isKnown)
			return Unknown;

		// else its just an or
		return (T)(value | t2.value);
	}

	Nullable &operator|=(const Nullable &t2)
	{
		return operator=(*this | t2);
	}

	Nullable operator+(const Nullable &t2) const
	{
		// if either is not known the result is not known
		if (!isKnown || !t2.isKnown)
			return Unknown;

		// else its just addition
		return (T)(value + t2.value);
	}

	Nullable operator>>(int n) const
	{
		// if the source is not known the result is not known
		if (!isKnown)
			return Unknown;

		// else its just a simple shift
		return (T)(value >> n);
	}

	Nullable operator<<(int n) const
	{
		// if the source is not known the result is not known
		if (!isKnown)
			return Unknown;

		// else its just a simple shift
		return (T)(value << n);
	}

	Nullable operator-(const Nullable &t2) const
	{
		// if either is not known the result is not known
		if (!isKnown || !t2.isKnown)
			return Unknown;

		// else its just a subtraction
		return (T)(value - t2.value);
	}

	Nullable operator^(const Nullable &t2) const
	{
		// if either is not known the result is not known
		if (!isKnown || !t2.isKnown)
			return Unknown;

		// else its just an xor
		return (T)(value ^ t2.value);
	}

	Nullable operator&(const Nullable &t2) const
	{
		// if either is zero the result is zero
		if (isKnown && !value)
			return false;
		if (t2.isKnown && !t2.value)
			return false;

		// else if either is not known the result is not known
		if (!isKnown || !t2.isKnown)
			return Unknown;

		// else its just an and
		return (T)(value & t2.value);
	}

	T Value(void) const
	{
		if (!isKnown)
			throw MathBoxException("Attempt to access value of unknown nullable");
		return value;
	}

public:
	static Nullable Unknown;

private:
	T value;
	bool isKnown;
};





// =======================================================================
// =======================================================================
// =============== Nullable template class static members ================
// =======================================================================
// =======================================================================
Nullable<bool> Nullable<bool>::Unknown;
template <class T> Nullable<T> Nullable<T>::Unknown;




// =======================================================================
// =======================================================================
// =============== Nullable<bool> special implementations ================
// =======================================================================
// =======================================================================
template <> Nullable<bool> Nullable<bool>::operator~(void) const
{
	return operator!();
}

template <> Nullable<bool> Nullable<bool>::operator&&(const Nullable<bool> &b) const
{
	// if either value is false then so is our result
	if (isKnown && !value)
		return false;
	if (b.isKnown && !b.value)
		return false;

	// else if either is not known the result is not known
	if (!isKnown || !b.isKnown)
		return Unknown;

	// else its just an and
	return value & b.value;
}

template <> Nullable<bool> Nullable<bool>::operator||(const Nullable<bool> &b) const
{
	// if either value is true then so is our result
	if (isKnown && value)
		return true;
	if (b.isKnown && b.value)
		return true;

	// else if either is not known the result is not known
	if (!isKnown || !b.isKnown)
		return Unknown;

	// else its just an or
	return value | b.value;
}



// =======================================================================
// =======================================================================
// ===================== Standard Nullable types =========================
// =======================================================================
// =======================================================================
typedef class Nullable<bool>		Tristate;
typedef class Nullable<uint8_t>	NullableByte;
typedef class Nullable<Nybble>	NullableNybble;


#endif
