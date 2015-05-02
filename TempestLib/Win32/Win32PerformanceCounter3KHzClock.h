/* 
 * File:   Win32PerformanceCounter3KHzClock.h
 * Author: Randy
 *
 * Created on April 26, 2015, 1:43 PM
 */

#ifndef WIN32PERFORMANCECOUNTER3KHZCLOCK_H
#define	WIN32PERFORMANCECOUNTER3KHZCLOCK_H

#include "..\Abstract3KHzClock.h"

class Win32PerformanceCounter3KHzClock : public Abstract3KHzClock
{
public:
   Win32PerformanceCounter3KHzClock(void);
   
   virtual bool IsHigh(void);

private:
   uint64_t performanceCountsPerHalfCycle;
};


#endif	/* WIN32PERFORMANCECOUNTER3KHZCLOCK_H */

