// ====================================================================
// Asteroids emulation project
//    Author: Randy Rasmussen
//    Copyright: none... do what you will
//    Warranties: none... do what you will at your own risk
//
// File summary:
//    This is the class that emulates the hardware of Asteroids that
//    the 6502 talks to directly, i.e. the RAM, ROM and I/O.
// ====================================================================

#include "stdafx.h"
#include <memory.h>

#include "TempestIO/AbstractTempestIO.h"

#include "AbstractGameEnvironment.h"
#include "TempestCPU/TempestException.h"

#include "AsteroidsROMs.h"
#include "AsteroidsVectorInterpreter.h"

#include "AsteroidsBus.h"


AsteroidsBus::AsteroidsBus(AbstractGameEnvironment *_environment)
{
   // copy parameters
   environment = _environment;

   // clear
   memset(this->vectorRAM, 0, sizeof(this->vectorRAM));

   // allocate
   vectorDataSnapshotMutex = new std::mutex();

   // install our timers
   StartTimer(4000, [this]() { SetNMI(); });
   StartTimer(250, [this]() { Tick6KHz(); });
   StartTimer(15000, [this]() { CheckVectorRAM(); });

   // configure address space
   ConfigureAddressSpace();
}

AsteroidsBus::~AsteroidsBus(void)
{
   delete vectorDataSnapshotMutex, vectorDataSnapshotMutex = nullptr;
}


void AsteroidsBus::GetVectorData(AsteroidsVectorInterpreter &vectorData)
{
   std::lock_guard<std::mutex> lock(*vectorDataSnapshotMutex);
   vectorData.SetVectorRAM(vectorRAMSnapshot);
}


void AsteroidsBus::SetNMITimer(void)
{
   // generate the NMI
   if (!selfTest)
      SetNMI();
}

/// <summary>
/// The sound generator needs to know what our internal time is so that
/// it can synchronize the generated sound with the game's clock.  This
/// function does just that... tells the sound generator what our clock
/// says.
/// </summary>
void AsteroidsBus::Tick6KHz(void)
{
   // give the sound output its heartbeat
   soundOutput->SetTime(GetTotalClockCycles());
}

void AsteroidsBus::ConfigureAddressSpace(void)
{
   // burn all of our ROMs
   for (uint16_t offset = 0; offset < 0x800; ++offset)
      ConfigureAddressAsROM((uint16_t)(0x5000 + offset), ROM035127_02[offset]);
   for (uint16_t offset = 0; offset < 0x800; ++offset)
      ConfigureAddressAsROM((uint16_t)(0x7800 + offset), ROM035143_02[offset]);
   for (uint16_t offset = 0; offset < 0x800; ++offset)
      ConfigureAddressAsROM((uint16_t)(0x7000 + offset), ROM035144_02[offset]);
   for (uint16_t offset = 0; offset < 0x800; ++offset)
      ConfigureAddressAsROM((uint16_t)(0x6800 + offset), ROM035145_02[offset]);
   for (uint16_t offset = 0; offset < 0x800; ++offset)
      ConfigureAddressAsROM((uint16_t)(0xF800 + offset), ROM035143_02[offset]);

   // configure zero page RAM and the stack
   for (uint16_t address=0x0000; address<0x0200; ++address)
      ConfigureAddressAsRAM(address);

   // configure our banked pages
   for (uint16_t address=0x0200; address<0x0400; ++address)
      ConfigureAddress(address, 0, ReadBankedRAM, WriteBankedRAM);

   // configure vector RAM
   for (uint16_t address = 0x4000; address<0x4800; ++address)
      ConfigureAddress(address, 0, ReadVectorRAM, WriteVectorRAM);

   // configure memory mapped I/O
   // Note that some of these are actually read-only but tolerate writes
   // as a no-op, because the software queries them with shift instructions
   // followed by checking the carry... tolerate and ignore
   ConfigureAddress(0x2001, 0, Read3KHzClock, WriteAddressInvalid);
   ConfigureAddressAsROM(0x2002, 0x00); // vector HALT
   ConfigureAddress(0x2003, 0x00, ReadHyperspaceButton, WriteAddressNoOp); // hyperspace button
   ConfigureAddress(0x2004, 0x00, ReadFireButton, WriteAddressNoOp); // fire button
   ConfigureAddress(0x2005, 0x00, ReadAddressNormal, WriteAddressNoOp); // diagnostic step switch switch
   ConfigureAddress(0x2006, 0x00, ReadAddressNormal, WriteAddressNoOp); // slam switch
   ConfigureAddress(0x2007, 0, ReadSelfTestSwitch, WriteAddressNoOp); // self-test switch
   ConfigureAddress(0x2400, 0x00, ReadAddressNormal, WriteAddressNoOp); // left coin switch
   ConfigureAddress(0x2401, 0x00, ReadAddressNormal, WriteAddressNoOp); // center coin switch
   ConfigureAddress(0x2402, 0x00, ReadAddressNormal, WriteAddressNoOp); // right coin switch
   ConfigureAddress(0x2403, 0x00, ReadOnePlayerButton, WriteAddressNoOp); // one player start
   ConfigureAddress(0x2404, 0x00, ReadTwoPlayerButton, WriteAddressNoOp); // two player start
   ConfigureAddress(0x2405, 0x00, ReadThrustButton, WriteAddressNoOp); // thrust
   ConfigureAddress(0x2406, 0x00, ReadRotateRightButton, WriteAddressNoOp); // rotate right button
   ConfigureAddress(0x2407, 0x00, ReadRotateLeftButton, WriteAddressNoOp); // rotate left button
   ConfigureAddressAsROM(0x2800, 0x00); // sw 8&7, 0 = free play
   ConfigureAddressAsROM(0x2801, 0x00); // sw 6&5, coin options
   ConfigureAddress(0x2802, 0x00, ReadAddressNormal, WriteAddressNoOp); // sw 4&3, 0 = 4 ships
   ConfigureAddressAsROM(0x2803, 0x02); // sw 2&1, 2 = french
   ConfigureAddress(0x3000, 0, ReadAddressInvalid, WriteVectorGO); // vector GO
   ConfigureAddress(0x3200, 0, ReadAddressInvalid, Write3200);
   ConfigureAddressAsRAM(0x3400); // watchdog clear
   ConfigureAddress(0x3600, 0, ReadAddressInvalid, WriteExplosionOutput);
   ConfigureAddress(0x3A00, 0, ReadAddressInvalid, WriteThumpOutput);
   ConfigureAddress(0x3C00, 0, ReadAddressInvalid, WriteSaucerSoundEnable);
   ConfigureAddress(0x3C01, 0, ReadAddressInvalid, WriteSaucerFireSound);
   ConfigureAddress(0x3C02, 0, ReadAddressInvalid, WriteSaucerSoundSelect);
   ConfigureAddress(0x3C03, 0, ReadAddressInvalid, WriteThrustSoundEnable);
   ConfigureAddress(0x3C04, 0, ReadAddressInvalid, WriteShipFireSound);
   ConfigureAddress(0x3C05, 0, ReadAddressInvalid, WriteLifeSound);
   ConfigureAddress(0x3E00, 0, ReadAddressInvalid, WriteResetSound);
}

/// <summary>
/// This returns a value indicating whether or not the caller can read
/// </summary>
bool AsteroidsBus::IsVectorGo(void) const
{
   return vectorGo;
}

uint8_t AsteroidsBus::Read3KHzClock(AbstractBus *bus, uint16_t address)
{
   uint64_t clockHalfCycleCount = bus->GetTotalClockCycles() / 250;
   if (clockHalfCycleCount & 1)
      return 0x80;
   else
      return 0;
}

uint8_t AsteroidsBus::ReadSelfTestSwitch(AbstractBus *bus, uint16_t address)
{
   AsteroidsBus *asteroidsBus = static_cast<AsteroidsBus *>(bus);
   return asteroidsBus->selfTest ? 0x80 : 0;
}

uint8_t AsteroidsBus::ReadVectorRAM(AbstractBus *bus, uint16_t address)
{
   AsteroidsBus *asteroidsBus = static_cast<AsteroidsBus *>(bus);
   return asteroidsBus->vectorRAM[address - 0x4000];
}

void AsteroidsBus::CheckVectorRAM(void)
{
   // If it has been a while (as measured in clock cycles) since the last write to
   // vector RAM we can conclude that it is stable and take a snapshot of it so that
   // the display doesn't catch it in the middle of updating; 800 clock ticks appears
   // to be a trustworthy number.
   if (lastVectorSnapshotTime != lastVectorRAMWrite)
   {
      uint64_t now = GetTotalClockCycles();
      if (now - lastVectorRAMWrite > 800)
      {
         std::lock_guard<std::mutex> lock(*vectorDataSnapshotMutex);

         memcpy(vectorRAMSnapshot, vectorRAM, sizeof(vectorRAM));
         lastVectorSnapshotTime = lastVectorRAMWrite;
      }
   }
}

void AsteroidsBus::WriteVectorRAM(AbstractBus *bus, uint16_t address, uint8_t value)
{
   AsteroidsBus *asteroidsBus = static_cast<AsteroidsBus *>(bus);

   // take a snapshot of the vector RAM if it's stable
   asteroidsBus->CheckVectorRAM();

   // do the write and note the time
   asteroidsBus->lastVectorRAMWrite = asteroidsBus->GetTotalClockCycles();
   asteroidsBus->vectorRAM[address - 0x4000] = value;
}

uint8_t AsteroidsBus::ReadBankedRAM(AbstractBus *bus, uint16_t address)
{
   AsteroidsBus *asteroidsBus = static_cast<AsteroidsBus *>(bus);
   address &= 0x1FF;
   if (asteroidsBus->ramSel)
      address ^= 0x100;
   return asteroidsBus->bankedRAM[address];
}

void AsteroidsBus::WriteBankedRAM(AbstractBus *bus, uint16_t address, uint8_t value)
{
   AsteroidsBus *asteroidsBus = static_cast<AsteroidsBus *>(bus);
   address &= 0x1FF;
   if (asteroidsBus->ramSel)
      address ^= 0x100;
   asteroidsBus->bankedRAM[address] = value;
}

void AsteroidsBus::Write3200(AbstractBus *bus, uint16_t address, uint8_t value)
{
   AsteroidsBus *asteroidsBus = static_cast<AsteroidsBus *>(bus);
   asteroidsBus->controlPanel->SetButtonLED(ONE_PLAYER_BUTTON, (value & 2) != 0);
   asteroidsBus->controlPanel->SetButtonLED(TWO_PLAYER_BUTTON, (value & 1) != 0);
   asteroidsBus->ramSel = (value & 4) != 0;
}

const uint8_t SOUND_CHANNEL_THUMP = 0;

void AsteroidsBus::WriteThumpOutput(AbstractBus *bus, uint16_t address, uint8_t value)
{
   AsteroidsBus *asteroidsBus = static_cast<AsteroidsBus *>(bus);

   SoundChannelState state;
   state.SetWaveform(Noise4Bit);
   state.SetFrequency(200 + 10 * (value & 0xF));
   if (value & 0x10)
      state.SetVolume(255);
   else
      state.SetVolume(0);
   asteroidsBus->soundOutput->SetTime(asteroidsBus->GetTotalClockCycles());
   asteroidsBus->soundOutput->SetSoundChannelState(SOUND_CHANNEL_THUMP, state);
}

void AsteroidsBus::WriteVectorGO(AbstractBus *bus, uint16_t address, uint8_t value)
{
   AsteroidsBus *asteroidsBus = static_cast<AsteroidsBus *>(bus);

   // take a snapshot of the vector RAM if it's stable
   asteroidsBus->CheckVectorRAM();

   // set our GO flag if we have a valid snapshot; note that we don't trust snapshots
   // taken while the game is starting up... there are circumstances where it will send
   // a GO too early, such as if one of the player start buttons is down at power-on
   asteroidsBus->vectorGo = asteroidsBus->lastVectorSnapshotTime > 500000;
}


uint8_t AsteroidsBus::ReadOnePlayerButton(AbstractBus *bus, uint16_t address)
{
   AsteroidsBus *asteroidsBus = static_cast<AsteroidsBus *>(bus);
   bool pressed = (asteroidsBus->controlPanel->GetButtons() & ONE_PLAYER_BUTTON) != 0;
   return pressed ? 0xFF : 0x00;
}

uint8_t AsteroidsBus::ReadTwoPlayerButton(AbstractBus *bus, uint16_t address)
{
   AsteroidsBus *asteroidsBus = static_cast<AsteroidsBus *>(bus);
   bool pressed = (asteroidsBus->controlPanel->GetButtons() & TWO_PLAYER_BUTTON) != 0;
   return pressed ? 0xFF : 0x00;
}

uint8_t AsteroidsBus::ReadRotateLeftButton(AbstractBus *bus, uint16_t address)
{
   AsteroidsBus *asteroidsBus = static_cast<AsteroidsBus *>(bus);
   bool pressed = (asteroidsBus->controlPanel->GetButtons() & ROTATE_LEFT_BUTTON) != 0;
   return pressed ? 0xFF : 0x00;
}

uint8_t AsteroidsBus::ReadRotateRightButton(AbstractBus *bus, uint16_t address)
{
   AsteroidsBus *asteroidsBus = static_cast<AsteroidsBus *>(bus);
   bool pressed = (asteroidsBus->controlPanel->GetButtons() & ROTATE_RIGHT_BUTTON) != 0;
   return pressed ? 0xFF : 0x00;
}


void AsteroidsBus::WriteResetSound(AbstractBus *bus, uint16_t address, uint8_t value)
{
   AsteroidsBus *asteroidsBus = static_cast<AsteroidsBus *>(bus);
   asteroidsBus->soundOutput->AllSoundOff();
}
