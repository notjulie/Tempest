
#include "TempestDisco.h"

#include "TempestIO/AbstractTempestStream.h"

#include "SystemTime.h"
#include "usbd_cdc_vcp.h"

#include "TempestDiscoVectorIO.h"

#include "CommandMode.h"


static char commandBuffer[100];
static bool chargePlaying;
static uint32_t chargeStartMillisecondCount;

static void ServiceCharge(void);
static void ProcessCommands(void);
static void ProcessCommand(const char *command);


void RunCommandMode(void)
{
	for (;;)
	{
		// process input
		ProcessCommands();

    	// service the USB transmitter
    	VCP.Service();

    	// service our test sound
    	ServiceCharge();
	}
}

static void ProcessCommands(void)
{
	for (;;)
	{
		// check for input
		int b = USBStream.Read();
		if (b < 0)
			return;

		// echo
		USBStream.Write((uint8_t)b);

		// if it's a newline we have a command to process
		if (b=='\r' || b=='\n')
		{
			USBStream.WriteString("\r\n");
			ProcessCommand(commandBuffer);
			commandBuffer[0] = 0;
		}
		else if (strlen(commandBuffer) >= sizeof(commandBuffer) - 1)
		{
			USBStream.WriteString("Input buffer overflow\r\n");
			commandBuffer[0] = 0;
		}
		else
		{
			// append the character and continue
			char buf[2] = {(char)b, 0};
			strcat(commandBuffer, buf);
		}
	}
}

static void ProcessCommand(const char *command)
{
	if (strcasecmp(command, "charge") == 0)
	{
		chargePlaying = true;
		chargeStartMillisecondCount = GetMillisecondCount();
		return;
	}

	USBStream.WriteString("I don't do that.\r\n");
}

static void ServiceCharge(void)
{
	// never mind if we're not playing
	if (!chargePlaying)
		return;

	// set channel one to square wave
	IO.SetSoundChannelWaveform(1, 10);

	// just do this brainless
	uint32_t elapsed = GetMillisecondCount() - chargeStartMillisecondCount;
	switch (elapsed / 125)
	{
	case 0:
		IO.SetSoundChannelFrequency(1, 108);
		IO.SetSoundChannelVolume(1, 15);
		break;
	case 1:
		IO.SetSoundChannelFrequency(1, 81);
		IO.SetSoundChannelVolume(1, 15);
		break;
	case 2:
	case 5:
		IO.SetSoundChannelFrequency(1, 64);
		IO.SetSoundChannelVolume(1, 15);
		break;
	case 3:
	case 6:
	case 7:
	case 8:
		IO.SetSoundChannelFrequency(1, 53);
		IO.SetSoundChannelVolume(1, 15);
		break;
	case 4:
		IO.SetSoundChannelVolume(1, 0);
		break;

	default:
		chargePlaying = false;
		IO.SetSoundChannelVolume(1, 0);
		break;
	}
}
