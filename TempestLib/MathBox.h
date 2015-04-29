/* 
 * File:   MathBox.h
 * Author: Randy
 *
 * Created on April 27, 2015, 4:22 PM
 */

#ifndef MATHBOX_H
#define	MATHBOX_H

#include <string>

class MathBox
{
public:
	std::string GetErrorString(void) const { return error.size() == 0 ? "OK" : error; }
   uint8_t GetStatus(void);
   uint8_t Read1(void);
   uint8_t Read2(void);
   void Write(uint8_t address, uint8_t value);

private:
	void SetError(const std::string &_status);

private:
	std::string	error;
};

#endif	/* MATHBOX_H */

