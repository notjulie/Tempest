
#include "TempestDisco.h"

#include "SystemTime.h"
#include "SystemError.h"

#include "DualDAC.h"


void DualDAC::Init(void)
{
	// clear
	dmaRequested = false;

	// make sure the clock is enabled
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

   // both channels use GPIOA
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

   // and they both use DMA1
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

   // enable clock for TIM6, which we use to trigger both channels
   RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

   /* Once the DAC channel is enabled, the corresponding GPIO pin is automatically
      connected to the DAC converter. In order to avoid parasitic consumption,
      the GPIO pin should be configured in analog */
   GPIO_InitTypeDef GPIO_InitStructure;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
   GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
}


void DualDAC::StartOutput(uint32_t *buffer, int sampleCount, uint32_t usDuration)
{
	uint64_t totalClockCycles = (uint64_t)GetAPB1TimerClockSpeed() * (uint64_t)usDuration / 1000000;
	uint64_t cyclesPerSample64 = totalClockCycles / sampleCount;
	if ((cyclesPerSample64 >> 32) != 0)
		ReportSystemError(SYSTEM_ERROR_DAC_DURATION_TOO_LONG);
	uint32_t cyclesPerSample = (uint32_t)cyclesPerSample64;

	// set both DAC channels to a neutral state
   DAC_InitTypeDef DAC_InitStructure;
   DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
   DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
   DAC_Init(DAC_Channel_1, &DAC_InitStructure);
   DAC_Init(DAC_Channel_2, &DAC_InitStructure);

   /* Enable DAC Channels: Once the DAC channel is enabled, the pin is
      automatically connected to the DAC converter. */
   DAC_Cmd(DAC_Channel_1, ENABLE);
   DAC_Cmd(DAC_Channel_2, ENABLE);

   // set up our clock... we use
   TIM6->CR1 = 0;
   TIM6->PSC = cyclesPerSample >> 16;
   TIM6->ARR = cyclesPerSample / (TIM6->PSC + 1) - 1;
   TIM6->DIER = 0x100U;
   TIM6->CR2 = 0;
   TIM6->CR1 = 1;

   TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);
   TIM_DMACmd(TIM6, TIM_DMA_Update, ENABLE);

   // set up the DACs
   DAC_InitTypeDef dacInitStructure;
   dacInitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
   dacInitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
   dacInitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
   dacInitStructure.DAC_LFSRUnmask_TriangleAmplitude = 0;
   DAC_Init(DAC_Channel_1, &dacInitStructure);
   DAC_Init(DAC_Channel_2, &dacInitStructure);

   // set up the DMA stream
   DMA_InitTypeDef DMA_InitStructure;
   DMA_DeInit(DMA1_Stream6);
   DMA_InitStructure.DMA_Channel = DMA_Channel_7;
   DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&DAC->DHR12RD);
   DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)buffer;
   DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
   DMA_InitStructure.DMA_BufferSize = sampleCount;
   DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
   DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
   DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
   DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
   DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
   DMA_InitStructure.DMA_Priority = DMA_Priority_High;
   DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
   DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
   DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
   DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
   DMA_Init(DMA1_Stream6, &DMA_InitStructure);

   // Enable DMA stream
   DMA_Cmd(DMA1_Stream6, ENABLE);

   // Enable DAC Channels
   DAC_Cmd(DAC_Channel_1, ENABLE);
   DAC_Cmd(DAC_Channel_2, ENABLE);

   // Enable DMA for DAC Channels
   DAC_DMACmd(DAC_Channel_1, ENABLE);
   DAC_DMACmd(DAC_Channel_2, ENABLE);
   dmaRequested = true;
}


bool DualDAC::IsDMARunning(void)
{
	bool running = dmaRequested && DMA_GetFlagStatus(DMA1_Stream6, DMA_FLAG_TCIF6) == RESET;
	if (!running)
		dmaRequested = false;
	return running;
}

