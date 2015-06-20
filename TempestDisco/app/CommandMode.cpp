
#include "TempestDisco.h"

#include "TempestIO/AbstractTempestStream.h"

#include "usbd_cdc_vcp.h"

#include "CommandMode.h"


static char commandBuffer[100];

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
	USBStream.WriteString("I don't do that.\r\n");
}
