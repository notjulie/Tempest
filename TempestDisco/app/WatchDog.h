
#ifndef WATCHDOG_H
#define WATCHDOG_H

typedef void WatchdogInterruptFunction(void);

void InitializeWatchdog(void);
void ResetIndependentWatchdogCounter(void);
void AddWatchdogInterruptFunction(WatchdogInterruptFunction *function);
bool WatchdogHeartbeat(void);

#endif
