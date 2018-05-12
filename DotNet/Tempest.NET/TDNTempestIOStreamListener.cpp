

#include "Stdafx.h"

#include "TempestIO/TempestIOStreamListener.h"

#include "TDNTempestIOStreamListener.h"

namespace TempestDotNET {

	TDNTempestIOStreamListener::TDNTempestIOStreamListener(ITDNStreamProvider ^stream, TDNWin32TempestSoundIO ^tempestIO)
	{
      // clear
      terminated = false;

		// keep references to the managed objects so they don't get garbage collected
		this->stream = stream;
		this->tempestIO = tempestIO;

		// create our listener
      listener = new TempestIOStreamListener(
         stream->GetStream(),
         tempestIO->GetSoundOutput(),
         tempestIO->GetControlPanel()
         );

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
