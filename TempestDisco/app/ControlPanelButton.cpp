
#include "SystemTime.h"

#include "ControlPanelButton.h"


ControlPanelButton::ControlPanelButton(void)
{
	on = false;
	doubleTapOn = false;
	doubleTapHadFirstTap = false;
}


void ControlPanelButton::SetSample(bool state)
{
	// our purpose is to debounce the button state by holding it high
	// for a certain interval after it goes low
	const int BUTTON_HOLD_MS = 10;

	// nab the system time
	uint32_t now = GetMillisecondCount();

	// debounce
	bool newValue = state;
	uint32_t timeSinceLastOn = now - lastOnSampleTime;
	if (!state && on && timeSinceLastOn < BUTTON_HOLD_MS)
		newValue = true;

	// if the new state is on, then we update the button's on time
	if (state)
		lastOnSampleTime = now;

	// note falling edges
	if (on && !newValue)
	{
		doubleTapHadFirstTap = true;
		lastFallingEdgeTime = now;
		doubleTapOn = false;
	}

	// note rising edges
	if (!on && newValue)
	{
		if (doubleTapHadFirstTap)
			doubleTapOn = true;
	}

	// if it's too long since our last falling edge, forget about
	// pending double taps
	uint32_t timeSinceLastFallingEdge = now - lastFallingEdgeTime;
	if (timeSinceLastFallingEdge > 250)
		doubleTapHadFirstTap = false;

	// set the value
	on = newValue;
}

