
#ifndef DISCODAC_H
#define DISCODAC_H


class DiscoDAC
{
public:
	void Init(int channel);

	bool IsDMARunning(void);
	void SetChannelData(uint16_t value);
	void StartRamp(uint16_t from, uint16_t to, uint32_t usDuration);

private:
	bool dmaRequested;
	int channelIndex;
	int channelMask;
	TIM_TypeDef *timer;
	DMA_Stream_TypeDef *dmaStream;
	uint16_t rampBuffer[4096];
};


#endif
