
#ifndef WIN32TEMPESTENVIRONMENT_H
#define WIN32TEMPESTENVIRONMENT_H

#include "Win32.h"
#include "TempestCPU/AbstractTempestEnvironment.h"

#pragma warning(push)
#pragma warning(disable : 4820)	// padding in structures

class Win32TempestEnvironment : public AbstractTempestEnvironment
{
public:
	Win32TempestEnvironment(void);
	virtual ~Win32TempestEnvironment(void);

   virtual std::string GetDatabasePathName(void);
};

#pragma warning(pop)

#endif
