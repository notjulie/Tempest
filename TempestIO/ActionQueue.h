
#ifndef ACTIONQUEUE_H
#define ACTIONQUEUE_H

#include <deque>
#include <future>
#include <thread>

class ActionQueue final {
public:
   template <typename T> class Action {
   public:
      template <typename U> Action(U action) : task(action) {
      }

      std::packaged_task<T()> task;
   };

private:
   ActionQueue(void);

public:
   ~ActionQueue(void);
   ActionQueue(const ActionQueue &) = delete;
   ActionQueue &operator=(const ActionQueue &) = delete;
   static ActionQueue *Create(void);

   template <typename T> T ExecuteSynchronous(Action<T> action) {
      //std::packaged_task<T> task(action);
      std::future<T> result = action.task.get_future();

      {
         std::lock_guard<std::mutex> lock(qMutex);
         actions.push_back([&]() { action.task(); });
         qEvent.notify_all();
      }

      result.wait();
      return result.get();
   }

   void ExecuteAsynchronous(const std::function<void(void)> &action);

private:
   void ThreadEntry(void);

private:
   bool terminated = false;
   std::thread *thread = nullptr;
   std::mutex qMutex;
   std::deque<std::function<void(void)>> actions;
   std::condition_variable qEvent;
   std::mutex eventMutex;
};

#endif
