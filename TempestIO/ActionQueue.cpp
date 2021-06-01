// ====================================================================
// ActionQueue
//    Author: Randy Rasmussen
//    Copyright: none... do what you will
//    Warranties: none... do what you will at your own risk
//
// File summary:
//    This is a simple queue thread that allows serialization of
//    asynchronous operations so that they can be executed synchronously
//    or asynchronously in a manner such that they always execute in
//    the order in which they are enqueued.
// ====================================================================


#include "TempestIO.Headers.h"
#include "ActionQueue.h"

/// <summary>
/// Creates a new ActionQueue
/// </summary>
ActionQueue *ActionQueue::Create(void)
{
   return new ActionQueue();
}


/// <summary>
/// Initializes a new instance of class ActionQueue
/// </summary>
ActionQueue::ActionQueue(void)
{
   // create our thread
   thread = new std::thread([this]() { ThreadEntry(); });
}

/// <summary>
/// Terminates an ActionQueue after all of its actions have completed
/// </summary>
ActionQueue::~ActionQueue(void)
{
   // make ourself as terminated; but let the thread itself do it, just to make
   // timing clearer
   ExecuteSynchronous<void>([this]() { terminated = true; });

   // wait for the thread to fully exit
   thread->join();

   // delete
   delete thread;
}


/// <summary>
/// Enqueues an action
/// </summary>
void ActionQueue::ExecuteAsynchronous(const std::function<void(void)> &action)
{
   // enqueue it
   {
      std::lock_guard<std::mutex> lock(qMutex);
      actions.push_back(action);
   }

   // set our condition that wakes up the thread
   {
      std::lock_guard<std::mutex> eventLock(actionsInQueueMutex);
      haveActionsInQueue = true;
      actionsInQueueCondition.notify_all();
   }
}


/// <summary>
/// Our thread function
/// </summary>
void ActionQueue::ThreadEntry(void)
{
   while (!terminated)
   {
      // wait for something to show up in the queue
      {
         std::unique_lock<std::mutex> lock(actionsInQueueMutex);
         actionsInQueueCondition.wait(lock, [this]{ return haveActionsInQueue;});
      }

      // empty out the queue
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
         {
            haveActionsInQueue = false;
            break;
         }

         action();
      }
   }
}
