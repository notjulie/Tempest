
#ifndef TRISTATE_H
#define TRISTATE_H

class Tristate
{
public:
	Tristate(void);
	Tristate(bool value);

	bool IsUnknown(void) const;

	Tristate operator!(void);
	Tristate operator&&(const Tristate &t2) const;
	Tristate operator^(const Tristate &t2) const;
	operator bool(void) const;

public:
	static Tristate Unknown;

private:
	bool value;
	bool isKnown;
};

#endif
