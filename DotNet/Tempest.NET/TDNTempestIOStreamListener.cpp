

#include "Stdafx.h"

#include "TempestIO/TempestIOStreamListener.h"

#include "TDNTempestIOStreamListener.h"

namespace TempestDotNET {

	TDNTempestIOStreamListener::TDNTempestIOStreamListener(ITDNStreamProvider ^stream)
	{
      // clear
      terminated = false;

		// keep references to the managed objects so they don't get garbage collected
		this->stream = stream;

      // create our IO object
      tempestSoundIO = new Win32TempestSoundIO();

		// create our listener
      listener = new TempestIOStreamListener(
         stream->GetStream(),
         tempestSoundIO,
         tempestSoundIO
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
		delete listener, listener = nullptr;

      // delete the IO object
      delete tempestSoundIO, tempestSoundIO = nullptr;
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
