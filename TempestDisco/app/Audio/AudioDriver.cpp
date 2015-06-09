
#include "TempestDisco.h"

#include "SystemError.h"

#include "AudioDriver.h"



void AudioDriverInit(void)
{
	// make sure that the clock for I2S is configured... this should
	// happen in system_stm32f4xx.c where the rest of the clocks are
	// configured
	if ((RCC->CR & RCC_CR_PLLI2SRDY) == 0)
		ReportSystemError(SYSTEM_ERROR_I2SCLOCK_NOT_READY);

	// initialize the audio output
	EVAL_AUDIO_DeInit();
	EVAL_AUDIO_SetAudioInterface(AUDIO_INTERFACE_I2S);
	if (EVAL_AUDIO_Init(OUTPUT_DEVICE_HEADPHONE, 100, I2S_AudioFreq_48k) !=0)
		ReportSystemError(SYSTEM_ERROR_AUDIO_INIT_FAILURE);

	// play a stupid square wave... 480Hz, stereo
	static int16_t wave[200];
	for (int i=0; i<100; ++i)
	{
		wave[i] = -10000;
		wave[100+i] = 10000;
	}

	// this will start playing the buffer in an endless loop that will call
	// us on interrupts whenever half of the buffer has played... thus
	// double-buffering.
	EVAL_AUDIO_Play((uint16_t*)wave, sizeof(wave));
}


// "C" callback functions that get called from the interrupt handlers
extern "C" {

	void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size)
	{

	}

	void EVAL_AUDIO_HalfTransfer_CallBack(uint32_t pBuffer, uint32_t Size)
	{

	}


	uint32_t Codec_TIMEOUT_UserCallback(void)
	{
		// this gets called on startup... ignore
		return 0;
	}

	uint16_t EVAL_AUDIO_GetSampleCallBack(void)
	{
		// I think this gets called never
		return 0;
	}

};
