/* 
 * File:   TempestException.h
 * Author: Randy
 *
 * Created on April 25, 2015, 1:56 PM
 */

#ifndef TEMPESTEXCEPTION_H
#define	TEMPESTEXCEPTION_H

class TempestException
{
public:
   TempestException(const std::string &_message);
   virtual ~TempestException(void);
   virtual const char* what(void) const;
   
private:
   std::string message;
};

#endif	/* TEMPESTEXCEPTION_H */

