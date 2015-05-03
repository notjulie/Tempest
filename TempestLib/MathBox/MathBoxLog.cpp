
#include "stdafx.h"
#include "MathBoxException.h"
#include "MathBoxLog.h"


MathBoxLog::MathBoxLog(void)
{
}


std::string MathBoxLog::GetXML(void) const
{
	std::lock_guard<std::mutex> lock(theMutex);

	std::string result;
	result += "<MathBoxLog>";
	for (unsigned i = 0; i < entries.size(); ++i)
		result += entries[i].GetXML();
	result += "</MathBoxLog>";
	return result;
}

void MathBoxLog::AddEntry(const MathBoxLogEntry &entry)
{
	std::lock_guard<std::mutex> lock(theMutex);

	entries.push_back(entry);
	if (entries.size() > 100)
		entries.erase(entries.begin());
}
