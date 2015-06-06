
#ifndef SYSTEMERROR_H
#define SYSTEMERROR_H

#ifdef __cplusplus
	extern "C" {
#endif


typedef enum {
	// the values here are 3 digit values with each digit
	// preferably in the range 1-5; each digit is displayed
	// by flashing one of the LEDs... red for hundreds,
	// blue for tens, green for ones... that makes a nice
	// left-to-right readout on the board
	SYSTEM_ERROR_USB_RECEIVE_OVERFLOW = 123,
	SYSTEM_ERROR_USB_TRANSMIT_OVERFLOW = 124,
	SYSTEM_ERROR_EXIT_CALLED = 551,
	SYSTEM_ERROR_KILL_CALLED = 552
} SystemError;

void ReportSystemError(SystemError systemError);



#ifdef __cplusplus
	};
#endif

#endif
