/* 
 * File:   EEPROM.h
 * Author: Randy
 *
 * Created on April 26, 2015, 10:22 AM
 */

#ifndef EEPROM_H
#define	EEPROM_H

class EEPROM
{
public:
	void     WriteByte(uint16_t address, uint8_t value);
	uint8_t  ReadByte(void);
};

#endif	/* EEPROM_H */

