
#ifndef NYBBLE_H
#define NYBBLE_H

class Nybble
{
public:
	Nybble(void) { value = 0; }
	Nybble(uint8_t _b) { value = _b; }

	bool operator!=(const Nybble &_n) const { return value != _n.value; }
	Nybble operator&(const Nybble &_n) const { return (uint8_t)(value & _n.value); }
	Nybble operator&(int _n) const { return (uint8_t)(value & _n); }
	Nybble operator|(const Nybble &_n) const { return (uint8_t)(value | _n.value); }
	Nybble operator^(const Nybble &_n) const { return (uint8_t)(value ^ _n.value); }
	Nybble operator+(const Nybble &_n) const { return (uint8_t)((value + _n.value) & 0xF); }
	Nybble operator-(const Nybble &_n) const { return (uint8_t)((value - _n.value) & 0xF); }
	bool operator!(void) const { return value == 0; }
	Nybble operator~(void) const { return (uint8_t)~value; }

private:
	uint8_t value;
};

#endif
