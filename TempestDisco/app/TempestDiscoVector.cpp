
#include "TempestDisco.h"

#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usbd_cdc_vcp.h"

#include "Discovery/LED.h"
#include "TempestIO/TempestIOStreamListener.h"
#include "Vector/DiscoVector.h"
#include "Vector/TempestDiscoVectorIO.h"

#include "CommandMode.h"
#include "SystemError.h"
#include "SystemTime.h"
#include "WatchDog.h"

#include "TempestDiscoVector.h"


extern "C" {

	// Private variables
	__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;


	//Configure pins and clocks
	void hw_init()
	{
		// ---------- GPIO -------- //
		// GPIOD Periph clock enable,
	   // Need to enable GPIOA because that's where the UART pins are.
	   // (Some of the USB is also on that port, and usb modules turn it on later...
	   //  but anyway, UART started working correctly when I turned clock on first)
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	   // ------------- USB -------------- //
		USBD_Init(&USB_OTG_dev,
				 USB_OTG_FS_CORE_ID,
				 &USR_desc,
				 &USBD_CDC_cb,
				 &USR_cb);

		// set up the push button as an interrupt
		STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_EXTI);
	}


	int main(void)
	{
		// for now freeze timers during debug
		DBGMCU->APB1FZ = 0xFFFFFFFF;

		SystemInit();

		// create our object that passes USB data to the IO objects
		TempestIOStreamListener USBListener(&USBStream, &IO);

		hw_init();
		InitializeLEDs();

		// initialize our main counters, SysTick, etc.
		InitializeSystemTime();

		// initialize the vector driver
		Vector.Init();

		// initialize the watchdog just before we enter the main loop
		InitializeWatchdog();

		// main loop
		for(;;) {
			// reset the dog
			ResetIndependentWatchdogCounter();

			// check for serial break
			if (VCP.WasBreakReceived())
			{
				VCP.ClearBreak();
				RunCommandMode();
			}

			// this takes data that has been received from the USB port
			// and interprets it, passing it to the audio or video subsystems
	    	USBListener.Service();

	    	// service the USB transmitter
	    	VCP.Service();

	    	// service Vector
	    	Vector.Service();
		}

	    return 0;
	}

	void EXTI0_IRQHandler(void)
	{
	   // we need to clear line pending bit manually
	   EXTI_ClearITPendingBit(EXTI_Line0);

		ReportSystemError(SYSTEM_ERROR_HALT_REQUESTED);
	}

};


