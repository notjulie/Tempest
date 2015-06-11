
#include "TempestDisco.h"

#include "SystemError.h"

#include "AudioDriver.h"

// About the buffer size... I haven't looked to see what the DMA burst
// size is, but the sound doesn't loop well if it's not a nice binary number.
// I think it needs to be a multiple of 64, or at least 16.  A buffer size
// of 1024 16-bit words == 512 stereo frames, which is about 1/100th of a second
// at 48KHz sample rate.  Somewhere around that neighborhood seems reasonable.
static int16_t wave[TEMPEST_DISCO_SOUND_BUFFER_SAMPLE_COUNT];


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

	// play a stupid square wave
	/*for (int i=0; i<BUFFER_SIZE; ++i)
	{
		if ((i/128) & 1)
			wave[i] = -500;
		else
			wave[i] = 500;
	}*/

	// this will start playing the buffer in an endless loop that will call
	// us on interrupts whenever half of the buffer has played... thus
	// double-buffering.

	// NOTE: there's a factor of 4 error in the sample code... I'll
	// just compensate here
	EVAL_AUDIO_Play((uint16_t*)wave, sizeof(wave) * 4);
}


// "C" callback functions that get called from the interrupt handlers
extern "C" {

	void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size)
	{
		// this is the green heartbeat LED
		static int cycles = 0;
		++cycles;
		GPIO_WriteBit(LED_GREEN_GPIO_PORT, LED_GREEN_PIN, ((cycles/50) & 1) ? Bit_SET : Bit_RESET);
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
