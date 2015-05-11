
#include "stdafx.h"
#include "Win32.h"

#include "TempestException.h"

#include "Win32WaveBuffer.h"


Win32WaveBuffer::Win32WaveBuffer(int sampleCount)
{
	prepared = false;
	isPlaying = false;

	waveData.resize((unsigned)sampleCount);
	waveHeader.dwUser = (DWORD_PTR)this;
	waveHeader.lpData = (LPSTR)&waveData[0];
	waveHeader.dwFlags = 0;
	waveHeader.dwBufferLength = waveData.size() * sizeof(waveData[0]);
}


void Win32WaveBuffer::Prepare(HWAVEOUT waveOut)
{
	MMRESULT result = waveOutPrepareHeader(waveOut, &waveHeader, sizeof(waveHeader));
	if (result != MMSYSERR_NOERROR)
		throw TempestException("Win32WaveBuffer::Prepare waveOutPrepareHeader failed");
	prepared = true;
}

void Win32WaveBuffer::Unprepare(HWAVEOUT waveOut)
{
	if (!prepared)
		return;

	waveOutUnprepareHeader(waveOut, &waveHeader, sizeof(waveHeader));
	prepared = false;
}


void Win32WaveBuffer::Play(HWAVEOUT waveOut)
{
	MMRESULT result = waveOutWrite(waveOut, &waveHeader, sizeof(waveHeader));
	if (result != MMSYSERR_NOERROR)
		throw TempestException("Win32WaveBuffer::Play waveOutWrite failed");
	isPlaying = true;
}
