
#include "CPU6502Exception.h"


CPU6502Exception::CPU6502Exception(const std::string &_message)
{
	message = _message;
}

CPU6502Exception::~CPU6502Exception(void)
{
}

const char* CPU6502Exception::what(void) const
{
	return message.c_str();
}


