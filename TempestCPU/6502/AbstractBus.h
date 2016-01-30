/* 
 * File:   AbstractBus.h
 * Author: Randy
 *
 * Created on April 25, 2015, 2:11 PM
 */

#ifndef ABSTRACTBUS_H
#define	ABSTRACTBUS_H

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable : 4820)	// padding in structures
#endif

class AbstractBus
{
public:
   AbstractBus(void);
   virtual ~AbstractBus(void);

   virtual uint8_t ReadByte(uint16_t address) = 0;
   virtual void    WriteByte(uint16_t address, uint8_t value) = 0;

   uint64_t GetTotalClockCycles(void);
   void IncrementClockCycleCount(int clockCycles);

protected:
   typedef void TimerFunction(AbstractBus *bus);

protected:
   void StartTimer(uint32_t period, TimerFunction *f);

private:
   void UpdateTimers(void);

private:
   typedef struct BusTimer {
      uint64_t nextIteration;
      uint32_t period;
      TimerFunction *timerFunction;
   };

private:
   uint64_t totalClockCycles;
   uint64_t nextTimerTime;
   std::vector<BusTimer> timers;
};

#ifdef _WIN32
#pragma warning(pop)
#endif

#endif	/* ABSTRACTBUS_H */

