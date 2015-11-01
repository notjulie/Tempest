
#include "TempestDisco.h"
#include "ControlPanel.h"

static uint32_t encoder = 0;

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

	// PC6 and PC8 are the inputs for the encoder... no pullups on them
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}


void ServiceControlPanel(void)
{
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
	// for now just return the bit values
	uint8_t bits = 0;
	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6) == Bit_SET)
		bits += 2;
	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8) == Bit_SET)
		bits += 1;
	return bits;
}
