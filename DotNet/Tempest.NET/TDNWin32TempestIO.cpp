
#include "stdafx.h"

#include "VectorEnumerator.h"
#include "Win32TempestIO.h"

#include "TDNWin32TempestIO.h"


using namespace System;

namespace TempestDotNET {
	TDNWin32TempestIO::TDNWin32TempestIO(void)
	{
		tempestIO = new Win32TempestIO();
	}

	TDNWin32TempestIO::~TDNWin32TempestIO(void)
	{
		delete tempestIO, tempestIO = NULL;
	}

	VectorEnumerator ^TDNWin32TempestIO::GetVectorEnumerator(void)
	{
		std::vector<SimpleVector> vectorList;
		tempestIO->GetVectorList(vectorList);
		return gcnew VectorEnumerator(vectorList);
	}

};
