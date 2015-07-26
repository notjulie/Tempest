
#ifndef DUALDAC_H
#define DUALDAC_H

class DualDAC {
public:
	void Init(void);
	bool IsDMARunning(void);
	void StartOutput(uint32_t *buffer, int sampleCount, uint32_t usDuration);

private:
	bool dmaRequested;
};

#endif
