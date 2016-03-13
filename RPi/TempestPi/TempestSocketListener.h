
#ifndef TEMPESTSOCKETLISTENER_H
#define TEMPESTSOCKETLISTENER_H

class AbstractTempestEnvironment;

class TempestSocketListener {
   public:
      TempestSocketListener(AbstractTempestEnvironment *environment);
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
      AbstractTempestEnvironment *environment;
};


#endif

