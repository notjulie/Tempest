
#include "TempestDisco.h"
#include "SystemError.h"


// these are functions that the linker wants, starting with the first C++ file I added
extern "C" {

	int _getpid(void)
	{
		return 42;
	}

	void _exit(void)
	{
		ReportSystemError(SYSTEM_ERROR_EXIT_CALLED);
	}

	void _kill(void)
	{
		ReportSystemError(SYSTEM_ERROR_KILL_CALLED);
	}
};
