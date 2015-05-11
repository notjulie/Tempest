
#ifndef WIN32WAVESTREAMER_H
#define WIN32WAVESTREAMER_H

#include "Win32WaveBuffer.h"

class AbstractTempestWaveStream;

#pragma warning(push)
#pragma warning(disable : 4820)	// padding in structures

class Win32WaveStreamer
{
public:
	Win32WaveStreamer(void);
	~Win32WaveStreamer(void);

	std::string GetErrorString(void) const;
	void SetSource(AbstractTempestWaveStream *_source) { source = _source; }
	void Update(int msElapsed);

private:
	void	CallbackThread(void);
	void  FillBuffer(Win32WaveBuffer *buffer);
	void  ProcessFinishedBuffer(Win32WaveBuffer *buffer);
	void  ProcessUpdate(int msElapsed);

private:
	static LONG __stdcall CallbackThreadEntry(LPVOID pThis) { ((Win32WaveStreamer *)pThis)->CallbackThread(); return 0; }

private:
	AbstractTempestWaveStream *source;

	HANDLE callbackThread;
	DWORD callbackThreadID;
	HWAVEOUT waveOut;
	bool terminating;
	bool errorReported;
	std::string errorString;
	std::vector<int16_t> inputBuffer;

	Win32WaveBuffer buffer1;
	Win32WaveBuffer buffer2;
};


#pragma warning(pop)

#endif
