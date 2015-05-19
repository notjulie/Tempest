
#ifndef WIN32WAVESTREAMER_H
#define WIN32WAVESTREAMER_H

#include "TempestIO/SoundGenerator.h"

#include "Win32WaveBuffer.h"


#pragma warning(push)
#pragma warning(disable : 4820)	// padding in structures

class Win32WaveStreamer
{
public:
	Win32WaveStreamer(void);
	~Win32WaveStreamer(void);

	std::string GetErrorString(void) const;
	void Update(int msElapsed);

private:
	void	CallbackThread(void);
	void  FillBuffer(Win32WaveBuffer *buffer);
	void  ProcessFinishedBuffer(Win32WaveBuffer *buffer);
	void  ProcessUpdate(int msElapsed);

private:
	static LONG __stdcall CallbackThreadEntry(LPVOID pThis) { ((Win32WaveStreamer *)pThis)->CallbackThread(); return 0; }

private:
	HANDLE callbackThread;
	DWORD callbackThreadID;
	HWAVEOUT waveOut;
	bool terminating;
	bool errorReported;
	std::string errorString;
	std::vector<int16_t> inputBuffer;

	SoundGenerator	soundGenerator;

	Win32WaveBuffer buffer1;
	Win32WaveBuffer buffer2;
};


#pragma warning(pop)

#endif
