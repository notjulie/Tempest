
#include "TempestDisco.h"

#include "Audio/DiscoWaveStreamer.h"
#include "SystemError.h"

#include "ControlPanel.h"

#include "TempestDiscoAudioIO.h"


TempestDiscoAudioIO	IO;


void TempestDiscoAudioIO::SetSoundChannelFrequency(int channel, int frequency)
{
	DWS.SetChannelFrequency(channel, frequency);
}

void TempestDiscoAudioIO::SetSoundChannelVolume(int channel, int volume)
{
	DWS.SetChannelVolume(channel, volume);
}

void TempestDiscoAudioIO::SetSoundChannelWaveform(int channel, int waveform)
{
	DWS.SetChannelWaveform(channel, waveform);
}

void TempestDiscoAudioIO::Tick6KHz(void)
{
	DWS.Tick6KHz();
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
