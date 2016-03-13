
#ifndef ABSTRACTTEMPESTENVIRONMENT_H
#define ABSTRACTTEMPESTENVIRONMENT_H

class CommandLine;

class AbstractTempestEnvironment
{
public:
   virtual ~AbstractTempestEnvironment(void) {}

	virtual void Reset(void) = 0;
	virtual void Sleep(int ms) = 0;
	virtual void SynchronizeClock(uint64_t busMSCount) = 0;

	void RegisterCommand(const std::string &name, std::function<std::string(const CommandLine &)> handler);
	std::string ExecuteCommand(const CommandLine &command);
};

#endif
