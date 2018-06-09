/*
 * File:   TempestException.h
 * Author: Randy
 *
 * Created on April 25, 2015, 1:56 PM
 */

#ifndef TEMPESTEXCEPTION_H
#define	TEMPESTEXCEPTION_H

#include "VectorGameException.h"

class TempestException : public VectorGameException
{
public:
   TempestException(const std::string &_message) : VectorGameException(_message) {}
};

#endif	/* TEMPESTEXCEPTION_H */

