
#ifndef WIN32WAVESTREAMER_H
#define WIN32WAVESTREAMER_H


class Win32WaveStreamer
{
public:
	Win32WaveStreamer(void);
	~Win32WaveStreamer(void);

private:
	void	CallbackThread(void);

private:
	static LONG __stdcall CallbackThreadEntry(LPVOID pThis) { ((Win32WaveStreamer *)pThis)->CallbackThread(); return 0; }

private:
	HANDLE callbackThread;
	DWORD callbackThreadID;
	HWAVEOUT waveOut;
};


#endif
