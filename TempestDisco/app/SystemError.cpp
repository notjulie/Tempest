
#include "TempestDisco.h"

#include "TempestIO/TempestMemoryStream.h"

#include "SystemError.h"

static void CPUSpin(int milliseconds);


extern "C" {

	void ReportSystemError(SystemError systemError)
	{
		// all we do is take over the CPU, hopefully shut down anything
		// harmful, then blink forever
		__disable_irq();

		// call SystemInit in case we need to again
		SystemInit();

		// Configure PD12, PD13, PD14 and PD15 in output pushpull mode...
		// we can't assume this has been done, of course
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);      // The LEDs are on GPIOD
        GPIO_InitTypeDef  GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin = LED_GREEN_PIN|LED_ORANGE_PIN|LED_RED_PIN|LED_BLUE_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOD, &GPIO_InitStructure);

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


	void *_sbrk(intptr_t incr)
	{
		ReportSystemError(SYSTEM_ERROR_SBRK_CALLED);
		return NULL;
	}

	void __cxa_pure_virtual()
	{
		ReportSystemError(SYSTEM_ERROR_PURE_VIRTUAL_CALLED);
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

void SimpleMemoryStream::ReportBufferOverflow(void)
{
	ReportSystemError(SYSTEM_ERROR_STREAMOVERFLOW);
}

void operator delete  ( void* ptr )
{
	ReportSystemError(SYSTEM_ERROR_DELETE_CALLED);
}
