

#include "Stdafx.h"

#include "TempestIO/TempestIOStreamListener.h"
#include "TempestIO/TempestMemoryStream.h"

#include "TDNTempestIOStreamListener.h"

namespace TempestDotNET {

	TDNTempestIOStreamListener::TDNTempestIOStreamListener(TDNMemoryStream ^stream, TDNWin32TempestIO ^tempestIO)
	{
      // clear
      terminated = false;

		// keep references to the managed objects so they don't get garbage collected
		this->stream = stream;
		this->tempestIO = tempestIO;

		// create our listener
      listener = new TempestIOStreamListener(stream->GetStream(), tempestIO->GetIOObject());

      // create our thread that does the listening
      thread = gcnew Thread(gcnew ThreadStart(this, &TDNTempestIOStreamListener::ThreadEntry));
      thread->Start();
	}


	TDNTempestIOStreamListener::~TDNTempestIOStreamListener(void)
	{
      // stop the thread
      this->terminated = true;
      thread->Join();
      thread = nullptr;

      // delete the listener
		delete listener, listener = NULL;
	}

   void TDNTempestIOStreamListener::ThreadEntry(void)
   {
      while (!terminated)
      {
         listener->Service();
         Sleep(0);
      }
   }
};
