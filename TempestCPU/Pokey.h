/* 
 * File:   Pokey.h
 * Author: Randy
 *
 * Created on April 25, 2015, 11:14 PM
 */

#ifndef POKEY_H
#define	POKEY_H

class AbstractArcadeGameControlPanel;
class AbstractTempestSoundOutput;

#ifdef _WIN32
   #pragma warning(push)
   #pragma warning(disable : 4820)	// padding in structures
#endif


class Pokey
{
public:
	Pokey(int baseSoundChannel);
	virtual ~Pokey(void);
   void SetTempestIO(AbstractTempestSoundOutput *_soundOutput) { this->soundOutput = _soundOutput; }
   void SetControlPanel(AbstractArcadeGameControlPanel *_controlPanel) { this->controlPanel = _controlPanel; }

	uint8_t ReadByte(uint16_t address);
   void    WriteByte(uint16_t address, uint8_t value, uint64_t busTime);

	virtual uint8_t GetALLPOT(void) { return 0; }

protected:
   AbstractTempestSoundOutput * GetSoundOutput(void) { return soundOutput; }
   AbstractArcadeGameControlPanel *GetControlPanel(void) { return controlPanel; }

private:
   uint8_t ALLPOT = 0;
   uint8_t SKCTLS = 0;
   int baseSoundChannel = 0;
   SoundChannelState soundChannelState[4];
	AbstractTempestSoundOutput *soundOutput = nullptr;
   AbstractArcadeGameControlPanel *controlPanel = nullptr;
};

#ifdef _WIN32
   #pragma warning(pop)
#endif

#endif	/* POKEY_H */

