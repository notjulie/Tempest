
#include "TempestDisco.h"

#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usbd_cdc_vcp.h"

#include "Audio/AudioDriver.h"
#include "TempestIO/TempestIOStreamListener.h"
#include "Vector/DiscoVector.h"

#include "CommandMode.h"
#include "DiscoWaveStreamer.h"
#include "SystemError.h"
#include "TempestDiscoIO.h"
#include "WatchDog.h"

#include "main.h"

TempestDiscoIO	IO;
TempestIOStreamListener USBListener(&USBStream, &IO);


extern "C" {

	// Private variables
	volatile uint32_t time_var1, time_var2;
	__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;


	//Configure pins and clocks
	void hw_init()
	{
	   GPIO_InitTypeDef  GPIO_InitStructure;


		// ---------- SysTick timer -------- //
		if (SysTick_Config(SystemCoreClock / 1000)) {
			while (true)    // Capture error
			 ;
		}

		// ---------- GPIO -------- //
		// GPIOD Periph clock enable,
	   // Need to enable GPIOA because that's where the UART pins are.
	   // (Some of the USB is also on that port, and usb modules turn it on later...
	   //  but anyway, UART started working correctly when I turned clock on first)
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);      // The LEDs are on GPIOD


		// Configure PD12, PD13, PD14 and PD15 in output pushpull mode
		GPIO_InitStructure.GPIO_Pin = LED_GREEN_PIN|LED_ORANGE_PIN|LED_RED_PIN|LED_BLUE_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOD, &GPIO_InitStructure);



	   // ------------- USB -------------- //
		USBD_Init(&USB_OTG_dev,
				 USB_OTG_FS_CORE_ID,
				 &USR_desc,
				 &USBD_CDC_cb,
				 &USR_cb);

		// set up the push button as an interrupt
		STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_EXTI);
	}



	/*
	 * Called from systick handler
	 */
	void timing_handler()
	{
		if (time_var1)
			time_var1--;

		time_var2++;
	}



	/*
	 * Delay a number of systick cycles (1ms)
	 */
	void Delay(volatile uint32_t nCount)
	{
		time_var1 = nCount;
		while (time_var1)
		  ;
	}
	

	int main(void)
	{
		// for now freeze timers during debug
		DBGMCU->APB1FZ = 0xFFFFFFFF;

		SystemInit();

		hw_init();

		// initialize the audio driver
		AudioDriverInit();

		// initialize the vector driver
		Vector.Init();

		// initialize the watchdog just before we enter the main loop
		InitializeWatchdog();

		// main loop
		for(;;) {
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

	    	// let the wave streamer have its time slice
	    	DWS.Service();
		}

	    return 0;
	}

	void EXTI0_IRQHandler(void)
	{
		ReportSystemError(SYSTEM_ERROR_HALT_REQUESTED);
	}

};


