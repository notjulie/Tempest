

#include "CommandLine.h"

#include "AbstractGameEnvironment.h"


std::string AbstractGameEnvironment::ExecuteCommand(const CommandLine &command)
{
   CommandMap::iterator i = commands.find(command.Command());
   if (i == commands.end())
      return "Not implemented";
   return (*i).second(command);
}

void AbstractGameEnvironment::RegisterCommand(const std::string &name, CommandHandler handler)
{
   commands[name] = handler;
}
