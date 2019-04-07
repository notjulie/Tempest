
#include <thread>
#include "TempestIOStreamListener.h"
#include "TempestIOStreamProxy.h"
#include "GameContext.h"

SerializedGameContext::SerializedGameContext(void) 
{
   // create the streamlistener that connects to one side of the stream... this connects to the
   // actual control panel and sound output
   listener = new TempestIOStreamListener(
      stream.GetLeftSide(),
      &tempestSoundIO,
      &tempestSoundIO
   );

   // create the proxy that makes the stream look like a control panel and sound interface that
   // tempest can use
   proxy = new TempestIOStreamProxy(stream.GetRightSide());

   // set up the proxy as the object that Tempest will connect to
   this->controlPanelReader = proxy;
   this->soundOutput = proxy;

   // direct our keyboard input to the tempestSoundIO object
   this->controlPanelWriter = &tempestSoundIO;

   // and start our poll thread
   pollThread = new std::thread([this]() { PollListener(); });
}


SerializedGameContext::~SerializedGameContext(void)
{
   // stop the thread
   terminated = true;
   if (pollThread != nullptr)
   {
      ((std::thread *)pollThread)->join();
      delete (std::thread *)pollThread, pollThread = nullptr;
   }
   delete listener, listener = nullptr;
   delete proxy, proxy = nullptr;
}

void SerializedGameContext::PollListener(void)
{
   while (!terminated) {
      listener->Service();
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
   }
}




COMPortGameContext::COMPortGameContext(const std::string &portName)
   :
   comPortStream(portName.c_str())
{
   proxy = new TempestIOStreamProxy(&comPortStream);

   // set up the proxy as the object that Tempest will connect to
   this->controlPanelReader = proxy;
   this->soundOutput = proxy;

   // direct our keyboard input to a placebo... we aren't listening to the keyboard
   this->controlPanelWriter = &placebo;
}

COMPortGameContext::~COMPortGameContext(void)
{
   delete proxy, proxy = nullptr;
}



LoopbackGameContext::LoopbackGameContext(const std::string &port1, const std::string &port2)
   :
   comPort1Stream(port1.c_str()),
   comPort2Stream(port2.c_str())
{
   // create the streamlistener that connects to one side of the stream... this connects to the
   // actual control panel and sound output
   listener = new TempestIOStreamListener(
      &comPort1Stream,
      tempestSoundIO,
      tempestSoundIO
   );

   // create the proxy that makes the stream look like a control panel and sound interface that
   // tempest can use
   proxy = new TempestIOStreamProxy(&comPort2Stream);

   // set up the proxy as the object that Tempest will connect to
   this->controlPanelReader = proxy;
   this->soundOutput = proxy;

   // direct our keyboard input to the tempestSoundIO object
   this->controlPanelWriter = tempestSoundIO;

   // and start our poll thread
   pollThread = new std::thread([this]() { PollListener(); });
}

LoopbackGameContext::~LoopbackGameContext(void)
{
   // stop the thread
   terminated = true;
   if (pollThread != nullptr)
   {
      ((std::thread *)pollThread)->join();
      delete (std::thread *)pollThread, pollThread = nullptr;
   }
   delete listener, listener = nullptr;
   delete proxy, proxy = nullptr;
}

void LoopbackGameContext::PollListener(void)
{
   while (!terminated) {
      listener->Service();
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
   }
}
