
#include "TempestDisco.h"

#include "Discovery/LED.h"
#include "TempestIO/TempestMemoryStream.h"

#include "SystemError.h"

static void CPUSpin(int milliseconds);
static void DisplaySystemError(SystemError systemError);

static SystemError theSystemError = SYSTEM_ERROR_NONE;

void SystemErrorInit(void)
{
	// NOTE: this gets called before the global memory is cleared
	// and the runtime is initialized... don't do anything too
	// crazy

	// get the CSR
	uint32_t resetFlags = RCC->CSR;

	// clear the reset flags
	RCC->CSR |= (1<<24);

	// check for watchdog timeout reset
	if (resetFlags & (1<<30))
		ReportSystemError(SYSTEM_ERROR_WINDOW_WATCHDOG_TIMEOUT);
	if (resetFlags & (1<<29))
		ReportSystemError(SYSTEM_ERROR_INDEPENDENT_WATCHDOG_TIMEOUT);

	// check to see if we are restarting due to a software reset in order
	// to report a system error
	if (resetFlags & (1<<28))
	{
		if (theSystemError != SYSTEM_ERROR_NONE)
		{
			SystemError e = theSystemError;
			theSystemError = SYSTEM_ERROR_NONE;
			DisplaySystemError(e);
		}
	}
}

extern "C" {

	void ReportSystemError(SystemError systemError)
	{
		// record the system error
		theSystemError = systemError;

		// do a software reset... the error will get displayed
		// after the reset so that we trust we are in a safe state
		SCB->AIRCR = 0x05FA0000 | SCB_AIRCR_SYSRESETREQ_Msk;
	}


	void __cxa_pure_virtual()
	{
		ReportSystemError(SYSTEM_ERROR_PURE_VIRTUAL_CALLED);
	}
};


static void DisplaySystemError(SystemError systemError)
{
	// all we do is take over the CPU, hopefully shut down anything
	// harmful, then blink forever
	__disable_irq();

	// call SystemInit to set up the clocks, then initialize the LEDs
	SystemInit();
	InitializeLEDs(false);

	for (;;)
	{
		int hundreds = systemError / 100;
		int tens = (systemError / 10) % 10;
		int ones = systemError % 10;

		while (hundreds>0 || tens>0 || ones>0)
		{
			LEDOn(DISCO_LED_RED, hundreds-- > 0);
			LEDOn(DISCO_LED_BLUE, tens-- > 0);
			LEDOn(DISCO_LED_GREEN, ones-- > 0);
			CPUSpin(200);

			LEDOn(DISCO_LED_RED, false);
			LEDOn(DISCO_LED_BLUE, false);
			LEDOn(DISCO_LED_GREEN, false);
			CPUSpin(200);
		}

		CPUSpin(800);
	}
}

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

void operator delete  ( void* ptr )
{
	ReportSystemError(SYSTEM_ERROR_DELETE_CALLED);
}
