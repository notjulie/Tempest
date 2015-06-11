
#ifndef WIN32WAVESTREAMER_H
#define WIN32WAVESTREAMER_H

#include "TempestIO/WaveStreamer.h"

#include "Win32WaveBuffer.h"


#pragma warning(push)
#pragma warning(disable : 4820)	// padding in structures


class Win32WaveStreamer : public WaveStreamer
{
public:
	Win32WaveStreamer(void);
	~Win32WaveStreamer(void);

	std::string GetErrorString(void) const;

protected:
   virtual void  QueueEvent(const WaveStreamEvent &event);

private:
	void	CallbackThread(void);

private:
	static LONG __stdcall CallbackThreadEntry(LPVOID pThis) { ((Win32WaveStreamer *)pThis)->CallbackThread(); return 0; }

private:
	HANDLE callbackThread;
	DWORD callbackThreadID;
	HWAVEOUT waveOut;
	HANDLE queueEvent;

	bool terminating;
	bool errorReported;
	std::string errorString;

	Win32WaveBuffer buffer1;
	Win32WaveBuffer buffer2;
};


#pragma warning(pop)

#endif
