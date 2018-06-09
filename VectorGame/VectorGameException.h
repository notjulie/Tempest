
#ifndef VECTORGAMEEXCEPTION_H
#define VECTORGAMEEXCEPTION_H

#include <string>

class VectorGameException
{
public:
   VectorGameException(const std::string &_message) : message(_message) {}
   virtual ~VectorGameException(void) {}
   virtual const char* what(void) const { return message.c_str(); }

private:
   std::string message;
};


#endif
