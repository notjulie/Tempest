
#ifndef TEMPEST_H
#define TEMPEST_H

class CPU6502;
class TempestBus;
class Win32PerformanceCounter3KHzClock;

namespace TempestDotNET {

	public ref class Tempest
	{
	public:
		Tempest();
		~Tempest();

		void Start(void);

	private:
		CPU6502		*cpu6502;
		TempestBus	*tempestBus;
		Win32PerformanceCounter3KHzClock *clock;
	};

}

#endif
