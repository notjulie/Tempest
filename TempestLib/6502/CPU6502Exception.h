
#ifndef CPU6502EXCEPTION_H
#define CPU6502EXCEPTION_H

#include <string>

class CPU6502Exception
{
public:
	CPU6502Exception(const std::string &_message);
	virtual ~CPU6502Exception(void);
	virtual const char* what(void) const;

private:
	std::string message;
};

#endif

