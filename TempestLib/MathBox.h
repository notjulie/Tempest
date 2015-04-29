/* 
 * File:   MathBox.h
 * Author: Randy
 *
 * Created on April 27, 2015, 4:22 PM
 */

#ifndef MATHBOX_H
#define	MATHBOX_H

class MathBox
{
public:
   uint8_t GetStatus(void);
   uint8_t Read1(void);
   uint8_t Read2(void);
   uint8_t Write(uint8_t address, uint8_t value);
};

#endif	/* MATHBOX_H */

