
#include "TempestIO.Headers.h"
#include "ActionQueue.h"

ActionQueue *ActionQueue::Create(void)
{
   return new ActionQueue();
}


ActionQueue::ActionQueue(void)
{
   // create our thread
   thread = new std::thread([this]() { ThreadEntry(); });
}

ActionQueue::~ActionQueue(void)
{
   // shut down the thread
   terminated = true;
   ExecuteAsynchronous([]() {});
   thread->join();
   delete thread;
}

void ActionQueue::ExecuteAsynchronous(const std::function<void(void)> &action)
{
   std::lock_guard<std::mutex> lock(qMutex);
   actions.push_back(action);
   {
      std::lock_guard<std::mutex> eventLock(eventMutex);
      qEvent.notify_all();
   }
}

void ActionQueue::ThreadEntry(void)
{
   while (!terminated)
   {
      {
         std::unique_lock<std::mutex> lock(eventMutex);
         qEvent.wait(lock);
      }

      for (;;)
      {
         std::function<void()> action;
         bool haveAction = false;
         {
            std::lock_guard <std::mutex> qLock(qMutex);
            if (actions.size() > 0)
            {
               action = actions.front();
               actions.pop_front();
               haveAction = true;
            }
         }

         if (!haveAction)
            break;
         action();
      }
   }
}
