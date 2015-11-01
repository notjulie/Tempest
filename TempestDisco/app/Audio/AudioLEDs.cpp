
#include "TempestDisco.h"

#include "USB/usbd_cdc_vcp.h"

#include "AudioDriver.h"
#include "ControlPanel.h"
#include "DiscoWaveStreamer.h"
#include "LED.h"
#include "WatchDog.h"

#include "AudioLEDs.h"


void ServiceAudioLEDs(void)
{
	int blueIntensity = 0;
	int redIntensity = 0;
	int greenIntensity = 0;
	int orangeIntensity = 0;

	if (DWS.HaveSoundOutput())
		++blueIntensity;
	if (GetUSBReceiveHeartbeat())
		++redIntensity;
	if (AudioDriverHeartbeat())
		++greenIntensity;
	if (WatchdogHeartbeat())
		++orangeIntensity;

	if (GetButton(FIRE_BUTTON))
		++orangeIntensity;
	if (GetButton(ZAPPER_BUTTON))
		++redIntensity;
	if (GetButton(ONE_PLAYER_BUTTON))
		++greenIntensity;
	if (GetButton(TWO_PLAYER_BUTTON))
		++blueIntensity;
	
	LEDValue(DISCO_LED_BLUE, 0x2000 * blueIntensity);
	LEDValue(DISCO_LED_RED, 0x2000 * redIntensity);
	LEDValue(DISCO_LED_GREEN, 0x2000 * greenIntensity);
	LEDValue(DISCO_LED_ORANGE, 0x0800 * orangeIntensity);
}
