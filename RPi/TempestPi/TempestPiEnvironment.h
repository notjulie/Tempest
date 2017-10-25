
#ifndef TEMPESTPIENVIRONMENT_H
#define TEMPESTPIENVIRONMENT_H

#include "TempestCPU/AbstractTempestEnvironment.h"

class TempestPiEnvironment : public AbstractTempestEnvironment
{
public:
   TempestPiEnvironment(void);

	virtual std::string GetDatabasePathName(void);
};


#endif
