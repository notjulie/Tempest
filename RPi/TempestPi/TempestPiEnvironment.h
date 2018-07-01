
#ifndef TEMPESTPIENVIRONMENT_H
#define TEMPESTPIENVIRONMENT_H

#include "AbstractGameEnvironment.h"

class TempestPiEnvironment : public AbstractGameEnvironment
{
public:
   TempestPiEnvironment(void);

	virtual std::string GetDatabasePathName(void);
};


#endif
