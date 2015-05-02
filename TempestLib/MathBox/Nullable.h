
#ifndef NULLABLE_H
#define NULLABLE_H

class Tristate;

template<class T> class Nullable
{
public:
	Nullable(void) { isKnown = false; }
	Nullable(T _value) { isKnown = true; value = _value; }

	bool IsUnknown(void) const { return !isKnown; }

	Tristate operator!=(const Nullable &_other);

	Nullable operator|(const Nullable &t2) const
	{
		// if either is not known the result is not known
		if (!isKnown || !t2.isKnown)
			return Unknown;

		// else its just an or
		return (T)(value | t2.value);
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


template <class T> Nullable<T> Nullable<T>::Unknown;

typedef class Nullable<uint8_t> NullableByte;

class Tristate : public Nullable<bool>
{
public:
	Tristate(void);
	Tristate(bool value);
	Tristate(const Nullable<bool> value);

	Tristate operator!(void) const;
	Tristate operator&&(const Tristate &t2) const;
	Tristate operator||(const Tristate &t2) const;
};


template <class T> Tristate Nullable<T>::operator!=(const Nullable<T> &_other)
{
	// if either is unknown we don't know if they are unequal
	if (!isKnown || _other.IsUnknown())
		return Tristate::Unknown;
	else
		return value != _other.Value();
}


#endif
