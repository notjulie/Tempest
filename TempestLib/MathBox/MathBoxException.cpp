
#include "stdafx.h"
#include "MathBoxException.h"

MathBoxException::MathBoxException(const std::string &_message)
{
	message = _message;
}

MathBoxException::~MathBoxException(void)
{
}

const char* MathBoxException::what(void) const
{
	return message.c_str();
}


