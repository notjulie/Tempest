
#include "TempestDisco.h"

#include "Audio/AudioDriver.h"
#include "Audio/TempestDiscoAudioIO.h"
#include "TempestIO/AbstractTempestStream.h"
#include "TempestIO/TempestIOStreamListener.h"

#include "LED.h"

#include "AudioLEDs.h"
#include "ControlPanel.h"
#include "DiscoWaveStreamer.h"
#include "Serial.h"
#include "SystemError.h"
#include "SystemTime.h"
#include "TechPort.h"
#include "WatchDog.h"

#include "TempestDiscoAudio.h"


static AppState appState;
static uint32_t lastUSBInputTime;

static void ServiceConnected(void);
static void ServiceUnconnected(void);

static AbstractTempestStream *tempestStream = NULL;
static TempestIOStreamListener *tempestListener = NULL;

extern "C" {

	//Configure pins and clocks
	void hw_init()
	{
		// ---------- GPIO -------- //
		// GPIOD Periph clock enable,
	   // Need to enable GPIOA because that's where the UART pins are.
	   // (Some of the USB is also on that port, and usb modules turn it on later...
	   //  but anyway, UART started working correctly when I turned clock on first)
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
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

		// serial
		InitializeSerial();

		// tech port
		Tech.Initialize();

		// our listener that handles data transmission between us and Tempest
		tempestStream = &SerialStream;
		TempestIOStreamListener listener(tempestStream, &IO, &IO);
		tempestListener = &listener;

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

			// serial
			ServiceSerial();

			// kick the dog
	    	ResetIndependentWatchdogCounter();
		}

	   return 0;
	}
};


static void ServiceConnected(void)
{
	// check for disconnect
	uint32_t now = GetMillisecondCount();
	if (tempestStream->Peek() < 0)
	{
		uint32_t idleTime = now - lastUSBInputTime;
		if (idleTime > 100)
		{
			// we went too long without receiving something from the other end;
			// switch to unconnected and kill all sound
			appState = UNCONNECTED;
			IO.AllSoundOff();
			return;
		}
	}
	else
	{
		lastUSBInputTime = now;
	}

	// this takes data that has been received from Tempest
	// and interprets it, passing it to the audio subsystem or control panel
	tempestListener->Service();
}

static void ServiceUnconnected(void)
{
	// see if we have a connection
	if (tempestStream->Peek() >= 0)
	{
		lastUSBInputTime = GetMillisecondCount();
		appState = CONNECTED;
	}
}

AppState GetAppState(void)
{
	return appState;
}
