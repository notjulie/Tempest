
#ifndef COMMANDLINE_H
#define COMMANDLINE_H

class CommandLine {
   public:
      CommandLine(const std::string &commandString);

      std::string Command(void) const { return command; }

   private:
      std::string commandString;
      std::string command;
};

#endif
