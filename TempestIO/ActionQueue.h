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


#ifndef ACTIONQUEUE_H
#define ACTIONQUEUE_H

#include <deque>
#include <future>
#include <thread>

/// <summary>
/// class ActionQueue; serializes actions on a background thread
/// </summary>
class ActionQueue final {
public:
   /// <summary>
   /// class Action; this is a simple template for allowing any Callable to be passed to us
   /// </summary>
   template <typename T> class Action {
   public:
      template <typename U> Action(U action) : task(action) {
      }

      std::packaged_task<T()> task;
   };

private:
   /// <summary>
   /// constructor; this is left private, because thread classes need to be consciously
   /// destructed rather than just falling out of scope; allocating this via new reinforces that
   /// </summary>
   ActionQueue(void);

public:
   ~ActionQueue(void);
   ActionQueue(const ActionQueue &) = delete;
   ActionQueue &operator=(const ActionQueue &) = delete;
   static ActionQueue *Create(void);

   /// <summary>
   /// Executes the action asynchronously but waits for it to complete
   /// </summary>
   template <typename T> T ExecuteSynchronous(Action<T> action) {
      // get the future that will tell us when the action has been carried out
      std::future<T> result = action.task.get_future();

      // execute asynchronously
      ExecuteAsynchronous([&]() { action.task(); });

      // wait and return
      result.wait();
      return result.get();
   }

   /// <summary>
   /// Executes the action asynchronously
   /// </summary>
   void ExecuteAsynchronous(const std::function<void(void)> &action);

private:
   void ThreadEntry(void);

private:
   bool terminated = false;
   std::thread *thread = nullptr;
   std::mutex qMutex;
   std::deque<std::function<void(void)>> actions;

   bool haveActionsInQueue = false;
   std::condition_variable actionsInQueueCondition;
   std::mutex actionsInQueueMutex;
};

#endif
