/* 
 * File:   Pokey.h
 * Author: Randy
 *
 * Created on April 25, 2015, 11:14 PM
 */

#ifndef POKEY_H
#define	POKEY_H

class AbstractTempestSoundIO;

#pragma warning(push)
#pragma warning(disable : 4820)	// padding in structures


class Pokey
{
public:
	Pokey(int baseSoundChannel);
	virtual ~Pokey(void);
	void SetTempestIO(AbstractTempestSoundIO *_tempestIO) { tempestIO = _tempestIO; }

	uint8_t ReadByte(uint16_t address);
	void    WriteByte(uint16_t address, uint8_t value);

	virtual uint8_t GetALLPOT(void) { return 0; }

protected:
   AbstractTempestSoundIO *GetIO(void) { return tempestIO; }

private:
	uint8_t ALLPOT;
	int baseSoundChannel;
	AbstractTempestSoundIO *tempestIO;
};


#pragma warning(pop)

#endif	/* POKEY_H */

