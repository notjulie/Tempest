
#include "stdafx.h"

#include "CPU6502Exception.h"

#include "AbstractBus.h"

AbstractBus::AbstractBus(void)
{
   // clear
   irq = false;
   isPaused = false;
   totalClockCycles = 0;
   nextTimerTime = (uint64_t)(int64_t)-1;
}


AbstractBus::~AbstractBus(void)
{
}

void AbstractBus::IncrementClockCycleCount(uint32_t clockCycles)
{
   totalClockCycles += clockCycles;
   if (totalClockCycles >= nextTimerTime)
   {
      for (unsigned i = 0; i < timers.size(); ++i)
      {
         if (totalClockCycles >= timers[i].nextIteration)
         {
            timers[i].timerFunction(this);
            timers[i].nextIteration += timers[i].period;
         }
      }

      UpdateTimers();
   }
}

uint64_t AbstractBus::GetTotalClockCycles(void)
{
   return totalClockCycles;
}

uint8_t AbstractBus::ReadByte(uint16_t address)
{
   return memory[address].reader(this, address);
}

void AbstractBus::WriteByte(uint16_t address, uint8_t value)
{
   return memory[address].writer(this, address, value);
}

void AbstractBus::StartTimer(uint32_t period, TimerFunction *f)
{
   BusTimer timer;
   timer.period = period;
   timer.nextIteration = totalClockCycles + period;
   timer.timerFunction = f;
   timers.push_back(timer);

   UpdateTimers();
}

void AbstractBus::UpdateTimers(void)
{
   uint64_t soonestTimerTime = (uint64_t)(int64_t)-1;
   for (unsigned i = 0; i < timers.size(); ++i)
      if (timers[i].nextIteration < soonestTimerTime)
         soonestTimerTime = timers[i].nextIteration;
   nextTimerTime = soonestTimerTime;
}

void AbstractBus::ConfigureAddress(uint16_t address, uint8_t value, ReadFunction *reader, WriteFunction *writer)
{
   memory[address].reader = reader;
   memory[address].writer = writer;
   memory[address].value = value;
}

void AbstractBus::ConfigureAddressAsRAM(uint16_t address)
{
   ConfigureAddress(address, 0, ReadAddressNormal, WriteAddressNormal);
}

uint8_t AbstractBus::ReadAddressInvalid(AbstractBus *, uint16_t address)
{
   char buffer[200];
   sprintf(buffer, "Invalid read address: %X", address);
   throw CPU6502Exception(buffer);
}

uint8_t AbstractBus::ReadAddressNormal(AbstractBus *bus, uint16_t address)
{
   return bus->memory[address].value;
}

void AbstractBus::WriteAddressInvalid(AbstractBus *, uint16_t address, uint8_t)
{
   char  buffer[200];
   sprintf(buffer, "Invalid write address: 0x%X", address);
   throw CPU6502Exception(buffer);
}

void AbstractBus::WriteAddressNormal(AbstractBus *bus, uint16_t address, uint8_t value)
{
   bus->memory[address].value = value;
}
