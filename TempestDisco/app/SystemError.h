
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
	SYSTEM_ERROR_NONE = 0,
	SYSTEM_ERROR_PURE_VIRTUAL_CALLED = 111,
	SYSTEM_ERROR_AUDIO_INIT_FAILURE = 112,
	SYSTEM_ERROR_HALT_REQUESTED = 113,
	SYSTEM_ERROR_USB_TRANSMIT_OVERFLOW = 124,
	SYSTEM_ERROR_DELETE_CALLED = 125,
	SYSTEM_ERROR_HARD_FAULT = 131,
	SYSTEM_ERROR_MEMORY_MANAGEMENT_ERROR = 132,
	SYSTEM_ERROR_BUS_FAULT = 133,
	SYSTEM_ERROR_USAGE_FAULT = 134,
	SYSTEM_ERROR_SYSTICK_FAIL = 141,
	SYSTEM_ERROR_I2SCLOCK_NOT_READY = 211,
	SYSTEM_ERROR_DAC_DURATION_TOO_LONG = 212,
	SYSTEM_ERROR_INDEPENDENT_WATCHDOG_TIMEOUT = 213,
	SYSTEM_ERROR_WINDOW_WATCHDOG_TIMEOUT = 214,
	SYSTEM_ERROR_UNIMPLEMENTED_FUNCTION = 333,
	SYSTEM_ERROR_CODEC_TIMEOUT = 411,
	SYSTEM_ERROR_GET_SAMPLE = 412,
	SYSTEM_ERROR_TOO_MANY_WATCHDOG_FUNCTIONS = 413,
	SYSTEM_ERROR_EXIT_CALLED = 551,
	SYSTEM_ERROR_KILL_CALLED = 552
} SystemError;


void SystemErrorInit(void);
void ReportSystemError(SystemError systemError);



#ifdef __cplusplus
	};
#endif

#endif
