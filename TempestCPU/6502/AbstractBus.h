// ====================================================================
// 6502 emulation project
//    Author: Randy Rasmussen
//    Copyright: none... do what you will
//    Warranties: none... do what you will at your own risk
//
// File summary:
//    This is the abstraction of everything that the 6502 will interact
//    with.
// ====================================================================

#ifndef ABSTRACTBUS_H
#define	ABSTRACTBUS_H

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable : 4820)	// padding in structures
#endif

#include <functional>
#include <vector>

class AbstractBus
{
public:
   AbstractBus(void);
   virtual ~AbstractBus(void);

   uint8_t ReadByte(uint16_t address);
   void    WriteByte(uint16_t address, uint8_t value);

   uint64_t GetTotalClockCycles(void);
   void IncrementClockCycleCount(uint32_t clockCycles);
   bool IsIRQ(void) { return irq; }
   bool IsNMI(void) { return nmi; }
   void SetNMI(void) { this->nmi = true; }
   void ClearNMI(void) { nmi = false; }
   void StartTimer(uint32_t period, const std::function<void()> &function);

protected:
   typedef uint8_t ReadFunction(AbstractBus *bus, uint16_t address);
   typedef void WriteFunction(AbstractBus *bus, uint16_t address, uint8_t value);

protected:
   void ConfigureAddress(uint16_t address, uint8_t value, ReadFunction *reader, WriteFunction *writer);
   void ConfigureAddressAsRAM(uint16_t address);
   void ConfigureAddressAsROM(uint16_t address, uint8_t value);
   void SetIRQ(bool _irq) { this->irq = _irq; }

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
      std::function<void()> timerFunction;
   };
   struct MemoryAttributes {
      ReadFunction *reader = &ReadAddressInvalid;
      WriteFunction *writer = &WriteAddressInvalid;
      uint8_t value = 0;
   };

private:
   bool irq = false;
   bool nmi = false;
   uint64_t totalClockCycles = 0;
   uint64_t nextTimerTime = (uint64_t)(int64_t)-1;
   std::vector<BusTimer> timers;
   MemoryAttributes  memory[64*1024];
};

#ifdef _WIN32
#pragma warning(pop)
#endif

#endif	/* ABSTRACTBUS_H */

