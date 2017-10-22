
#ifndef ABSTRACTTEMPESTENVIRONMENT_H
#define ABSTRACTTEMPESTENVIRONMENT_H

class CommandLine;

class AbstractTempestEnvironment
{
public:
   typedef std::function<std::string(const CommandLine &)> CommandHandler;

public:
   virtual ~AbstractTempestEnvironment(void) {}

	virtual void Reset(void) = 0;
	virtual void Sleep(int ms) = 0;
	virtual void SynchronizeClock(uint64_t busMSCount) = 0;
   virtual std::string GetDatabasePathName(void) = 0;

	void RegisterCommand(const std::string &name, CommandHandler handler);
	std::string ExecuteCommand(const CommandLine &command);

private:
   typedef std::map<std::string, CommandHandler> CommandMap;

private:
   CommandMap commands;
};

#endif
