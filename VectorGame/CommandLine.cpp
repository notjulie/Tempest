
#include "stdafx.h"
#include "CommandLine.h"

CommandLine::CommandLine(const std::string &commandString)
{
   // save the original
   this->commandString = commandString;

   // parse out just the command
   size_t spacePos = commandString.find_first_of(' ');
   if (spacePos == std::string::npos)
      command = commandString;
   else
      command = commandString.substr(0, spacePos);
}
