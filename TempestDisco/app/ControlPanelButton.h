
#ifndef CONTROLPANELBUTTON_H
#define CONTROLPANELBUTTON_H

#include <stdint.h>

class ControlPanelButton
{
public:
	ControlPanelButton(void);
	void SetSample(bool sample);

	bool IsOn(void) const { return on; }
	bool IsDoubleTapOn(void) const { return doubleTapOn; }

private:
	bool  on;
	uint32_t lastOnSampleTime;

	// double tap state
	bool doubleTapOn;
	bool doubleTapHadFirstTap;
	uint32_t lastFallingEdgeTime;
};


#endif
