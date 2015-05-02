
#ifndef NULLABLE_H
#define NULLABLE_H

template<class T> class Nullable
{
public:
	Nullable(void) { isKnown = false; }
	Nullable(T _value) { isKnown = true; value = _value; }

	bool IsUnknown(void) const { return !isKnown; }

	Nullable operator|(const Nullable &t2) const
	{
		// if either is not known the result is not known
		if (!isKnown || !t2.isKnown)
			return Unknown;

		// else its just an or
		return value | t2.value;
	}

	Nullable operator^(const Nullable &t2) const
	{
		// if either is not known the result is not known
		if (!isKnown || !t2.isKnown)
			return Unknown;

		// else its just an xor
		return value ^ t2.value;
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
		return value && t2.value;
	}

	operator T(void) const
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


template <class T> Nullable<T> Nullable<T>::Unknown;

typedef class Nullable<uint8_t> NullableByte;

#endif
