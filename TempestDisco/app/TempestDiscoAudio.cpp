
#include "TempestDisco.h"

#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usbd_cdc_vcp.h"

#include "Audio/AudioDriver.h"
#include "Audio/TempestDiscoAudioIO.h"
#include "TempestIO/AbstractTempestStream.h"
#include "TempestIO/TempestIOStreamListener.h"

#include "LED.h"

#include "AudioLEDs.h"
#include "ControlPanel.h"
#include "DiscoWaveStreamer.h"
#include "SystemError.h"
#include "SystemTime.h"
#include "WatchDog.h"

#include "TempestDiscoAudio.h"


static AppState appState;

static void ServiceConnected(void);
static void ServiceUnconnected(void);

TempestIOStreamListener USBListener(&USBStream, &IO);


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

		hw_init();
      InitializeLEDs(true);

		// initialize our main counters, SysTick, etc.
		InitializeSystemTime();

		// initialize the audio driver
		AudioDriverInit();

		// control panel
		InitializeControlPanel();

		// initialize the watchdog just before we enter the main loop
		InitializeWatchdog();

		// main loop
		for(;;) {
			// process according to state
			switch (appState)
			{
			case UNCONNECTED:
				ServiceUnconnected();
				break;

			case CONNECTED:
				ServiceConnected();
				break;
			}

	    	// let the wave streamer have its time slice
	    	DWS.Service();

	    	// update the LEDs
	    	ServiceAudioLEDs();

			// control panel
			ServiceControlPanel();

			// kick the dog
	    	ResetIndependentWatchdogCounter();
		}

	   return 0;
	}

	void EXTI0_IRQHandler(void)
	{
		static int testBeepVolume = 0;

	   // we need to clear line pending bit manually
	   EXTI_ClearITPendingBit(EXTI_Line0);

		// for now just start playing a note just to see if we can
		IO.SetSoundChannelWaveform(1, 10);	// square wave
		IO.SetSoundChannelFrequency(1, 108);
		IO.SetSoundChannelVolume(1, ++testBeepVolume & 15);
	}

};


static void ServiceConnected(void)
{
	// this takes data that has been received from the USB port
	// and interprets it, passing it to the audio or video subsystems
 	USBListener.Service();

 	// service the USB transmitter
 	VCP.Service();
}

static void ServiceUnconnected(void)
{
	// see if we have a connection
	if (USBStream.Peek() >= 0)
		appState = CONNECTED;
}

AppState GetAppState(void)
{
	return appState;
}
