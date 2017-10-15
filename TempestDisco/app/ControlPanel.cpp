
#include "TempestDisco.h"

#include "Encoder.h"
#include "SystemTime.h"

#include "ControlPanel.h"

struct ButtonState {
	ButtonState(void) { on = false; }
	bool  on;
	uint32_t onTime;
};

static ButtonState fireButton;
static ButtonState zapButton;
static ButtonState onePlayerButton;
static ButtonState twoPlayerButton;
static Encoder encoder;

static void ServiceButton(ButtonState *button, bool state);


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

	// enable the peripheral clock for GPIOE
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	// Configure PE4 and PE5 as open collector outputs, and set them high
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIOE->BSRRL = 0x30;

	// PB0 and PB1 are the inputs for the encoder
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// enable ADC1 and ADC2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);

	// set the clock prescale to 8... we can't possibly read it at its maximum speed
	// anyway
	ADC->CCR = 3 << 16;

	// set up ADC1 to read PB0 in continuous mode
	ADC1->CR1 = 0;
	ADC1->CR2 =
			(1 << 11) | // left align
			(1 << 1);   //continuous
	ADC1->SQR1 = 0; // sample only one channel
	ADC1->SQR2 = 0;
	ADC1->SQR3 = 8; // sample only channel 8, PB0
	ADC1->SMPR2 = 7; // 480 ADC cycles per conversion
	ADC1->CR2 |= 1; // enable ADC
	ADC1->CR2 |= (1<<30); // start

	// set up ADC2 to read PB1 in continuous mode
	ADC2->CR1 = 0;
	ADC2->CR2 =
			(1 << 11) | // left align
			(1 << 1);   //continuous
	ADC2->SQR1 = 0; // sample only one channel
	ADC2->SQR2 = 0;
	ADC2->SQR3 = 9; // sample only channel 9, PB1
	ADC2->SMPR2 = 7; // 480 ADC cycles per conversion
	ADC2->CR2 |= 1; // enable ADC
	ADC2->CR2 |= (1<<30); // start

	// enable the peripheral clock for TIM2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	// calculate our timer counts
	int ENCODER_SAMPLE_FREQUENCY = 20000;
	int timerCounts = GetAPB1TimerClockSpeed() / ENCODER_SAMPLE_FREQUENCY;
	int prescale = 1 + (timerCounts >> 16);
	timerCounts /= prescale;

	// enable TIM2 interrupts
	NVIC_InitTypeDef   NVIC_InitStructure;
   NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);

	// setup TIM2
	TIM2->PSC = prescale - 1;
	TIM2->ARR = timerCounts - 1;
	TIM2->DIER = 1;
	TIM2->CR1 = 1;
}


void ServiceControlPanel(void)
{
	ServiceButton(&onePlayerButton, GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_3) == Bit_RESET);
	ServiceButton(&twoPlayerButton, GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_0) == Bit_RESET);
	ServiceButton(&fireButton, GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1) == Bit_RESET);
	ServiceButton(&zapButton, GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2) == Bit_RESET);
}


static void ServiceButton(ButtonState *button, bool state)
{
	// our purpose is to debounce the button state by holding it high
	// for a certain interval after it goes low
	const int BUTTON_HOLD_MS = 10;

	// nab the system time
	uint32_t now = GetMillisecondCount();

	// if the new state is on, then we update the button's on time
	if (state)
	{
		button->on = true;
		button->onTime = now;
		return;
	}

	// the new state is off... if we're already off that's fine
	if (!button->on)
		return;

	// we can only turn it off if the hold time has elapsed
	uint32_t elapsed = now - button->onTime;
	if (elapsed >= BUTTON_HOLD_MS)
		button->on = false;
}

bool GetButton(ButtonFlag button)
{
	switch (button)
	{
	case ONE_PLAYER_BUTTON:
		return onePlayerButton.on;

	case TWO_PLAYER_BUTTON:
		return twoPlayerButton.on;

	case FIRE_BUTTON:
		return fireButton.on;

	case ZAPPER_BUTTON:
		return zapButton.on;

	default:
		return false;
	}
}

uint16_t GetEncoder(void)
{
	return encoder.GetValue();
}


void SetButtonLED(ButtonFlag button, bool value)
{
	// the LEDs are at PE4 (one player) and PE5 (two player)
	switch (button)
	{
	case ONE_PLAYER_BUTTON:
		if (value)
			GPIOE->BSRRL = 0x20;
		else
			GPIOE->BSRRH = 0x20;
		break;

	case TWO_PLAYER_BUTTON:
		if (value)
			GPIOE->BSRRL = 0x10;
		else
			GPIOE->BSRRH = 0x10;
		break;

	default:
		break;
	}
}


extern "C" {

void TIM2_IRQHandler(void)
{
	// clear the interrupt
	TIM2->SR = 0;

	// read the ADCs...
	int adc1 = ADC1->DR;
	int adc2 = ADC2->DR;

	// process
	encoder.AddSample(adc1, adc2);
}

};
