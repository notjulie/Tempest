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

   virtual uint8_t ReadByte(uint16_t address);
   virtual void    WriteByte(uint16_t address, uint8_t value);

   bool IsPaused(void) { return isPaused; }
   uint64_t GetTotalClockCycles(void);
   void IncrementClockCycleCount(uint32_t clockCycles);
   bool IsIRQ(void) { return irq; }
   void SetIsPaused(bool isPaused) { this->isPaused = isPaused; }

protected:
   typedef uint8_t ReadFunction(AbstractBus *bus, uint16_t address);
   typedef void TimerFunction(AbstractBus *bus);
   typedef void WriteFunction(AbstractBus *bus, uint16_t address, uint8_t value);

protected:
   void ConfigureAddress(uint16_t address, uint8_t value, ReadFunction *reader, WriteFunction *writer);
   void ConfigureAddressAsRAM(uint16_t address);
   void SetIRQ(bool irq) { this->irq = irq; }
   void StartTimer(uint32_t period, TimerFunction *f);

protected:
   static uint8_t ReadAddressInvalid(AbstractBus *bus, uint16_t address);
   static uint8_t ReadAddressNormal(AbstractBus *bus, uint16_t address);
   static void WriteAddressInvalid(AbstractBus *bus, uint16_t address, uint8_t value);
   static void WriteAddressNoOp(AbstractBus *, uint16_t, uint8_t) {}
   static void WriteAddressNormal(AbstractBus *bus, uint16_t address, uint8_t value);

private:
   void UpdateTimers(void);

private:
   struct BusTimer {
      uint64_t nextIteration;
      uint32_t period;
      TimerFunction *timerFunction;
   };
   struct MemoryAttributes {
      MemoryAttributes(void) {
         reader = &ReadAddressInvalid;
         writer = &WriteAddressInvalid;
      }

      ReadFunction *reader;
      WriteFunction *writer;
      uint8_t value;
   };

private:
   bool irq;
   bool isPaused;
   uint64_t totalClockCycles;
   uint64_t nextTimerTime;
   std::vector<BusTimer> timers;
   MemoryAttributes  memory[64*1024];
};

#ifdef _WIN32
#pragma warning(pop)
#endif

#endif	/* ABSTRACTBUS_H */

