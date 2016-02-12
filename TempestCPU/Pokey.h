/* 
 * File:   Pokey.h
 * Author: Randy
 *
 * Created on April 25, 2015, 11:14 PM
 */

#ifndef POKEY_H
#define	POKEY_H

class AbstractTempestSoundIO;

#ifdef _WIN32
   #pragma warning(push)
   #pragma warning(disable : 4820)	// padding in structures
#endif


class Pokey
{
public:
	Pokey(int baseSoundChannel);
	virtual ~Pokey(void);
	void SetTempestIO(AbstractTempestSoundIO *_tempestIO) { tempestIO = _tempestIO; }

	uint8_t ReadByte(uint16_t address);
   void    WriteByte(uint16_t address, uint8_t value, uint64_t busTime);

	virtual uint8_t GetALLPOT(void) { return 0; }

protected:
   AbstractTempestSoundIO *GetIO(void) { return tempestIO; }

private:
   uint8_t ALLPOT;
   uint8_t SKCTLS;
   int baseSoundChannel;
   SoundChannelState soundChannelState[4];
	AbstractTempestSoundIO *tempestIO;
};

#ifdef _WIN32
   #pragma warning(pop)
#endif

#endif	/* POKEY_H */

