
#include "stdafx.h"
#include "TempestException.h"

TempestException::TempestException(const std::string &_message)
{
   message = _message;
}

TempestException::~TempestException(void)
{
}

const char* TempestException::what(void) const
{
   return message.c_str();
}

