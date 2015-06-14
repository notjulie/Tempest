
#include "TempestDisco.h"
#include "DiscoDAC.h"


void DiscoDAC::Init(int _channel)
{
	// save the parameter
	channelIndex = _channel;

	// initialize things
	channelMask = (channelIndex==1) ? DAC_Channel_1 : DAC_Channel_2;

	// regardless of the channel we need to make sure the clock is enabled
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

   // and the both use GPIOA
   RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

   /* Once the DAC channel is enabled, the corresponding GPIO pin is automatically
      connected to the DAC converter. In order to avoid parasitic consumption,
      the GPIO pin should be configured in analog */
   GPIO_InitTypeDef GPIO_InitStructure;
   GPIO_InitStructure.GPIO_Pin =  (channelIndex == 1) ? GPIO_Pin_4 : GPIO_Pin_5;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_Init(GPIOA, &GPIO_InitStructure);

   DAC_InitTypeDef DAC_InitStructure;
   DAC_InitStructure.DAC_Trigger = DAC_Trigger_None ;
   DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
   DAC_Init(channelMask, &DAC_InitStructure);

   /* Enable DAC Channel: Once the DAC channel is enabled, the pin is
      automatically connected to the DAC converter. */
   DAC_Cmd(channelMask, ENABLE);

   // set it to something
   SetChannelData(0x4000);
}


void DiscoDAC::SetChannelData(uint16_t value)
{
	if (channelIndex == 1)
	   DAC_SetChannel1Data(DAC_Align_12b_L, value);
	else
	   DAC_SetChannel2Data(DAC_Align_12b_L, value);
}
