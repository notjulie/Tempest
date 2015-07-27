
#include "TempestDisco.h"

#include "TempestIO/AbstractTempestStream.h"

#include "DiscoRGB.h"
#include "usbd_cdc_vcp.h"

#include "TempestDiscoVectorIO.h"

#include "CommandMode.h"


static char commandBuffer[100];

static void ProcessCommands(void);
static void ProcessCommand(const char *command);


void ServiceCommandMode(void)
{
	// process input
	ProcessCommands();

	// service the USB transmitter
	VCP.Service();
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
	int rgbPulseIntensity = 1;

	if (strcasecmp(command, "r") == 0)
	{
		PulseRGB(rgbPulseIntensity, 0, 0, 5);
		return;
	}

	if (strcasecmp(command, "g") == 0)
	{
		PulseRGB(0, rgbPulseIntensity, 0, 5);
		return;
	}

	if (strcasecmp(command, "b") == 0)
	{
		PulseRGB(0, 0, rgbPulseIntensity, 5);
		return;
	}

	USBStream.WriteString("I don't do that.\r\n");
}

