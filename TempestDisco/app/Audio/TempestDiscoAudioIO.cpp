
#include "TempestDisco.h"

#include "Audio/DiscoWaveStreamer.h"
#include "SystemError.h"

#include "ControlPanel.h"

#include "TempestDiscoAudioIO.h"


TempestDiscoAudioIO	IO;


TempestDiscoAudioIO::TempestDiscoAudioIO(void)
{
	// clear
	cpuTime = 0;
}

void TempestDiscoAudioIO::SetSoundChannelState(int channel, SoundChannelState state)
{
	DWS.SetChannelState(channel, state);
}

void TempestDiscoAudioIO::SetTime(uint64_t clockCycles)
{
	DWS.Delay((int)(clockCycles - cpuTime));
   cpuTime = clockCycles;
}

uint8_t TempestDiscoAudioIO::GetButtons(void)
{
	uint8_t buttons = 0;
	if (GetButton(FIRE_BUTTON))
		buttons |= FIRE_BUTTON;
	if (GetButton(ZAPPER_BUTTON))
		buttons |= ZAPPER_BUTTON;
	if (GetButton(ONE_PLAYER_BUTTON))
		buttons |= ONE_PLAYER_BUTTON;
	if (GetButton(TWO_PLAYER_BUTTON))
		buttons |= TWO_PLAYER_BUTTON;
	return buttons;
}

uint8_t TempestDiscoAudioIO::GetEncoder(void)
{
	return (uint8_t)::GetEncoder();
}

void TempestDiscoAudioIO::SetButtonLED(ButtonFlag button, bool value)
{
	::SetButtonLED(button, value);
}
