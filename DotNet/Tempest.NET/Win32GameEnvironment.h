
#ifndef WIN32TEMPESTENVIRONMENT_H
#define WIN32TEMPESTENVIRONMENT_H

#include "AbstractGameEnvironment.h"

#pragma warning(push)
#pragma warning(disable : 4820)	// padding in structures

class Win32GameEnvironment : public AbstractGameEnvironment
{
public:
   Win32GameEnvironment(void);
	virtual ~Win32GameEnvironment(void);

   virtual std::string GetDatabasePathName(void);
};

#pragma warning(pop)

#endif
