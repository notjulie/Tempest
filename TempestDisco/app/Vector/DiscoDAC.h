
#ifndef DISCODAC_H
#define DISCODAC_H


class DiscoDAC
{
public:
	void Init(int channel);

	void SetChannelData(uint16_t value);

private:
	int channelIndex;
	int channelMask;
};


#endif
