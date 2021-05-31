
#ifndef VECTORGAMEEXCEPTION_H
#define VECTORGAMEEXCEPTION_H

#include <stdexcept>
#include <string>

class VectorGameException : public std::runtime_error
{
public:
   VectorGameException(const std::string &_message) : std::runtime_error(_message) {}
   virtual ~VectorGameException(void) {}
};


#endif
