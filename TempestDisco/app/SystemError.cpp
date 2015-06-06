
#include "TempestDisco.h"
#include "SystemError.h"

static void CPUSpin(int milliseconds);

extern "C" {

	void ReportSystemError(SystemError systemError)
	{
		// all we do is take over the CPU, hopefully shut down anything
		// harmful, then blink forever
		__disable_irq();

		for (;;)
		{
			int hundreds = systemError / 100;
			int tens = (systemError / 10) % 10;
			int ones = systemError % 10;

			while (hundreds>0 || tens>0 || ones>0)
			{
				GPIO_WriteBit(LED_RED_GPIO_PORT, LED_RED_PIN, (hundreds-- > 0) ? Bit_SET : Bit_RESET);
				GPIO_WriteBit(LED_BLUE_GPIO_PORT, LED_BLUE_PIN, (tens-- > 0) ? Bit_SET : Bit_RESET);
				GPIO_WriteBit(LED_GREEN_GPIO_PORT, LED_GREEN_PIN, (ones-- > 0) ? Bit_SET : Bit_RESET);
				CPUSpin(200);

				GPIO_WriteBit(LED_RED_GPIO_PORT, LED_RED_PIN, Bit_RESET);
				GPIO_WriteBit(LED_BLUE_GPIO_PORT, LED_BLUE_PIN, Bit_RESET);
				GPIO_WriteBit(LED_GREEN_GPIO_PORT, LED_GREEN_PIN, Bit_RESET);
				CPUSpin(200);
			}

			CPUSpin(800);
		}
	}

};


static void CPUSpin(int milliseconds)
{
	// since we have interrupts off and have a known CPU speed
	// we can just do a count loop for our delay... for a
	// system error it's best to stay simple

	// by the way... I don't claim this is accurate... there's nothing
	// time critical here, though
	for (int i=0; i<milliseconds; ++i)
		for (int j=0; j<20000; ++j)
			continue;
}

