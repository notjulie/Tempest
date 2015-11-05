
#include "TempestDisco.h"

#include "SystemTime.h"

#include "ControlPanel.h"

static uint32_t encoder = 0;
static uint8_t encoderBits = 0;

void InitializeControlPanel(void)
{
	// enable the peripheral clock for GPIOD
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	// Configure PD1, PD2, PD3 and PD4 as inputs with internal pull-ups
   GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	// PB0 and PB1 are the inputs for the encoder
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// set up ADC1 to read PB0 in continuous mode
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	ADC1->CR1 = 0; // no surprises here
	ADC1->CR2 =
			(1 << 11) | // left align
			(1 << 1);   //continuous
	ADC1->SQR1 = 0; // sample only one channel
	ADC1->SQR2 = 0;
	ADC1->SQR3 = 8; // sample only channel 8, PB0
	ADC1->CR2 |= 1; // enable ADC
	ADC1->CR2 |= (1<<30); // start

	// set up ADC2 to read PB1 in continuous mode
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
	ADC2->CR1 = 0; // no surprises here
	ADC2->CR2 =
			(1 << 11) | // left align
			(1 << 1);   //continuous
	ADC2->SQR1 = 0; // sample only one channel
	ADC2->SQR2 = 0;
	ADC2->SQR3 = 9; // sample only channel 9, PB1
	ADC2->CR2 |= 1; // enable ADC
	ADC2->CR2 |= (1<<30); // start
}


void ServiceControlPanel(void)
{
	// read the encoder bits
	uint8_t newEncoderBits = 0;
	if (ADC1->DR > 0x2000)
		newEncoderBits += 2;
	if (ADC2->DR > 0x2000)
		newEncoderBits += 1;
	//if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6) == Bit_SET)
		//newEncoderBits += 2;
	//if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8) == Bit_SET)
		//newEncoderBits += 1;

#if 0
	if (newEncoderBits != encoderBits)
	{
		// the encoder goes 1-3-2 or 2-3-1 depending on the direction;
		// combine the previous value and the current value and switch
		// on the combination
		uint8_t transition = (encoderBits<<4) + newEncoderBits;

		switch (transition)
		{
		case 0x13:
		//case 0x32:
			--encoder;
			break;

		case 0x23:
		//case 0x31:
			++encoder;
			break;

		default:
			encoder = encoder + 0;
			break;
		}

		// save the current state
		encoderBits = newEncoderBits;
	}
#else
	encoder = newEncoderBits;
#endif
}

bool GetButton(ButtonFlag button)
{
	switch (button)
	{
	case ONE_PLAYER_BUTTON:
		return GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_3) == Bit_RESET;

	case TWO_PLAYER_BUTTON:
		return GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_0) == Bit_RESET;

	case FIRE_BUTTON:
		return GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1) == Bit_RESET;

	case ZAPPER_BUTTON:
		return GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2) == Bit_RESET;

	default:
		return false;
	}
}

uint16_t GetEncoder(void)
{
	return encoder;
}
