
#ifndef LOGICBOARD_H
#define LOGICBOARD_H

#include "LogicSignal.h"


class LogicBoard
{
public:
   void Connect(LogicOutput &output, LogicInput &input);
   void PropogateSignals(void);

private:
   struct Update {
      LogicInput *target;
      bool newValue;
   };

private:
   std::map<LogicOutput*, LogicInput*> connections;
   std::queue<Update> updates;
};


#endif
