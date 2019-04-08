// ===============================================================
// WatchDog.h
//
//   Simple support for watchdog on STM32 processor.
//     Author: Randy Rasmussen
//     Copyright: None
//     Warranty: None
//
// ===============================================================

#ifndef WATCHDOG_H
#define WATCHDOG_H

/// <summary>
/// Definition of a function to be called on the interrupt generated
/// by the window watchdog.  Any functions that require critical periodic
/// monitoring can register with AddWatchdogInterruptFunction.
/// </summary>
typedef void WatchdogInterruptFunction(void);

/// <summary>
/// Starts both watchdogs; from this point forward ResetIntdependentWatchdogCounter
/// must be called periodically or the processor will be reset.
/// </summary>
void InitializeWatchdog(void);

/// <summary>
/// Resets the independent watchdog counter
/// </summary>
void ResetIndependentWatchdogCounter(void);

/// <summary>
/// Registers a function to be called on the window watchdog's interrupt
/// </summary>
void AddWatchdogInterruptFunction(WatchdogInterruptFunction *function);

/// <summary>
/// Returns the state of the watchdog's heartbeat pulse.
/// </summary>
bool WatchdogHeartbeat(void);

#endif
