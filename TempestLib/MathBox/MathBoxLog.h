
#ifndef MATHBOXLOG_H
#define MATHBOXLOG_H

#include "MathBoxLogEntry.h"

class MathBoxLog
{
public:
	MathBoxLog(void);

	void AddEntry(const MathBoxLogEntry &entry);
	std::string GetXML(void) const;

private:
	// forbidden constructors and such
	MathBoxLog(const MathBoxLog &log);
	MathBoxLog &operator=(const MathBoxLog &log);

private:
	std::vector<MathBoxLogEntry> entries;
	mutable std::mutex theMutex;
};

#endif
