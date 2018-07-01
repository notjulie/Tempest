
#ifndef TEMPESTSOCKETLISTENER_H
#define TEMPESTSOCKETLISTENER_H

class AbstractGameEnvironment;

class TempestSocketListener {
   public:
      TempestSocketListener(AbstractGameEnvironment *environment);
      ~TempestSocketListener(void);

   private:
      void ListenToClient(int client);
      void ListenForConnections(void);

   private:
      bool terminated;
      int theSocket;
      std::mutex clientListMutex;
      std::thread *connectionThread;
      std::vector<std::thread *> clientThreads;
      AbstractGameEnvironment *environment;
};


#endif

