
#ifndef DISCODAC_H
#define DISCODAC_H


class DiscoDAC
{
public:
	void Init(int channel);

	bool IsDMARunning(void);
	void SetChannelData(uint16_t value);
	void StartRamp(uint16_t *buffer, int sampleCount, uint32_t usDuration);

private:
	bool dmaRequested;
	int channelIndex;
	int channelMask;
	TIM_TypeDef *timer;
	DMA_Stream_TypeDef *dmaStream;
};


#endif
