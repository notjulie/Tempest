/* 
 * File:   MathBox.h
 * Author: Randy
 *
 * Created on April 27, 2015, 4:22 PM
 */

#ifndef MATHBOX_H
#define	MATHBOX_H

#include <string>
#include <vector>

class MathBox
{
public:
	void LoadROM(const uint8_t *rom, int length, char slot);

	std::string GetErrorString(void) const { return error.size() == 0 ? "OK" : error; }
   uint8_t GetStatus(void);
   uint8_t Read1(void);
   uint8_t Read2(void);
   void Write(uint8_t address, uint8_t value);

private:
	void SetError(const std::string &_status);

private:
	std::string	error;
	std::vector<uint8_t> romA;
	std::vector<uint8_t> romE;
	std::vector<uint8_t> romF;
	std::vector<uint8_t> romH;
	std::vector<uint8_t> romJ;
	std::vector<uint8_t> romK;
	std::vector<uint8_t> romL;
};

#endif	/* MATHBOX_H */

