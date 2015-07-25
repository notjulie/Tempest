
#include "TempestDisco.h"

#include "SystemTime.h"
#include "SystemError.h"

#include "DiscoDAC.h"


void DiscoDAC::Init(int _channel)
{
	// save the parameter
	channelIndex = _channel;

	// clear
	dmaRequested = false;

	// initialize things
	channelMask = (channelIndex==1) ? DAC_Channel_1 : DAC_Channel_2;

   // set up the timer... DAC1 uses TIM6, DAC2 uses TIM7
   if (_channel == 1)
   {
      // enable clock for TIM6
      RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
      timer = TIM6;
      dmaStream = DMA1_Stream6;
   }
   else
   {
      // enable clock for TIM7
      RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
      timer = TIM7;
      dmaStream = DMA1_Stream5;
   }

	// regardless of the channel we need to make sure the clock is enabled
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

   // and they both use GPIOA
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

   // and they both use DMA1
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

   /* Once the DAC channel is enabled, the corresponding GPIO pin is automatically
      connected to the DAC converter. In order to avoid parasitic consumption,
      the GPIO pin should be configured in analog */
   GPIO_InitTypeDef GPIO_InitStructure;
   GPIO_InitStructure.GPIO_Pin =  (channelIndex == 1) ? GPIO_Pin_4 : GPIO_Pin_5;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
}


bool DiscoDAC::IsDMARunning(void)
{
	bool running;
	if (channelIndex == 1)
		running = dmaRequested && DMA_GetFlagStatus(DMA1_Stream6, DMA_FLAG_TCIF6) == RESET;
	else
		running = dmaRequested && DMA_GetFlagStatus(DMA1_Stream5, DMA_FLAG_TCIF5) == RESET;

	if (!running)
		dmaRequested = false;

	return running;
}

void DiscoDAC::SetChannelData(uint16_t value)
{
	if (channelIndex == 1)
	   DAC_SetChannel1Data(DAC_Align_12b_L, value);
	else
	   DAC_SetChannel2Data(DAC_Align_12b_L, value);
}


void DiscoDAC::StartRamp(uint16_t from, uint16_t to, uint32_t usDuration)
{
	// create the ramp
	int index = 0;
	int value = from;
	for (;;)
	{
		rampBuffer[index++] = value;
		if (from < to)
		{
			if (++value > to)
				break;
		}
		else
		{
			if (--value < to)
				break;
		}
	}
	int sampleCount = index;

	uint64_t totalClockCycles = (uint64_t)GetAPB1TimerClockSpeed() * (uint64_t)usDuration / 1000000;
	uint64_t cyclesPerSample64 = totalClockCycles / sampleCount;
	if ((cyclesPerSample64 >> 32) != 0)
		ReportSystemError(SYSTEM_ERROR_DAC_DURATION_TOO_LONG);
	uint32_t cyclesPerSample = (uint32_t)cyclesPerSample64;

	// set up the DAC
   DAC_InitTypeDef DAC_InitStructure;
   DAC_InitStructure.DAC_Trigger = DAC_Trigger_None ;
   DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
   DAC_Init(channelMask, &DAC_InitStructure);

   /* Enable DAC Channel: Once the DAC channel is enabled, the pin is
      automatically connected to the DAC converter. */
   DAC_Cmd(channelMask, ENABLE);

   // set up our clock
   timer->CR1 = 0;
   timer->PSC = cyclesPerSample >> 16;
   timer->ARR = cyclesPerSample / (timer->PSC + 1) - 1;
   timer->DIER = 0x100U;
   timer->CR2 = 0;
   timer->CR1 = 1;

   TIM_SelectOutputTrigger(timer, TIM_TRGOSource_Update);
   TIM_DMACmd(timer, TIM_DMA_Update, ENABLE);

   // set up the DAC
   DAC_InitTypeDef dacInitStructure;
   dacInitStructure.DAC_Trigger = channelIndex == 1 ? DAC_Trigger_T6_TRGO : DAC_Trigger_T7_TRGO;
   dacInitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
   dacInitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
   dacInitStructure.DAC_LFSRUnmask_TriangleAmplitude = 0;
   DAC_Init(channelMask, &dacInitStructure);

   DMA_InitTypeDef DMA_InitStructure;
   DMA_DeInit(dmaStream);
   DMA_InitStructure.DMA_Channel = DMA_Channel_7;
   DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(channelIndex == 1 ? &DAC->DHR12R1 : &DAC->DHR12R2);
   DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)rampBuffer;
   DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
   DMA_InitStructure.DMA_BufferSize = sampleCount;
   DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
   DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
   DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
   DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
   DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
   DMA_InitStructure.DMA_Priority = DMA_Priority_High;
   DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
   DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
   DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
   DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
   DMA_Init(dmaStream, &DMA_InitStructure);

   // Enable DMA stream
   DMA_Cmd(dmaStream, ENABLE);

   // Enable DAC Channel
   DAC_Cmd(channelMask, ENABLE);

   // Enable DMA for DAC Channel
   DAC_DMACmd(channelMask, ENABLE);
   dmaRequested = true;
}

