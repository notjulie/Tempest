
#ifndef COMMANDLINE_H
#define COMMANDLINE_H

class CommandLine {
   public:
      CommandLine(const std::string &commandString);

   private:
      std::string commandString;
};

#endif
