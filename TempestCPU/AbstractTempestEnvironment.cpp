
#include "stdafx.h"

#include "CommandLine.h"

#include "AbstractTempestEnvironment.h"


std::string AbstractTempestEnvironment::ExecuteCommand(const CommandLine &command)
{
   CommandMap::iterator i = commands.find(command.Command());
   if (i == commands.end())
      return "Not implemented";
   return (*i).second(command);
}

void AbstractTempestEnvironment::RegisterCommand(const std::string &name, CommandHandler handler)
{
   commands[name] = handler;
}
