
#ifndef TEMPESTMEMORYMAP_H
#define TEMPESTMEMORYMAP_H


// ROM addresses
const uint16_t ROM_BASE = 0x9000;
const uint16_t ROM_ECHO_START = 0xE000;
const uint16_t ROM_ECHO_END = 0xFFFF;
const uint16_t ROM_ECHO_SOURCE = 0xC000;

// RAM addresses
const uint16_t MAIN_RAM_BASE = 0x0;
const uint16_t MAIN_RAM_SIZE = 0x800;
const uint16_t COLOR_RAM_BASE = 0x0800;
const uint16_t VECTOR_RAM_BASE = 0x2000;
const uint16_t VECTOR_ROM_BASE = 0x3000;
const uint16_t VECTOR_ROM_SIZE = 0x1000;

// peripheral addresses
const uint16_t POKEY1_BASE = 0x60C0;
const uint16_t POKEY1_END = 0x60CF;
const uint16_t POKEY2_BASE = 0x60D0;
const uint16_t POKEY2_END = 0x60DF;
const uint16_t EEPROM_WRITE_BASE = 0x6000;
const uint16_t EEPROM_WRITE_END = 0x603F;
const uint16_t MATHBOX_WRITE_BASE = 0x6080;
const uint16_t MATHBOX_WRITE_END = 0x609F;

// program variable addresses
const uint16_t PLAYER1_LEVEL_ADDRESS = 0x0046;
const uint16_t PLAYER2_LEVEL_ADDRESS = 0x0047;
const uint16_t COPY_PROTECTION_FLAG_ADDRESS = 0x011F;
const uint16_t MAX_START_LEVEL_ADDRESS = 0x0126;

// program addresses
const uint16_t CLEAR_PLAYER_SCORE_ROUTINE = 0xCA62;
const uint16_t INCREASE_PLAYER_SCORE_ROUTINE = 0xCA6C;
const uint16_t OUTPUT_HIGH_SCORE_ROUTINE = 0xAEBE;
const uint16_t OUTPUT_HIGH_SCORE_ROUTINE_EXIT = 0xAEC1;

// vector memory addresses
const uint16_t DISPLAY_PLAYER1_SCORE_ADDRESS = 0x2F6C;
const uint16_t DISPLAY_PLAYER2_SCORE_ADDRESS = 0x2FBE;
const uint16_t DISPLAY_HIGH_SCORE_ADDRESS = 0x2F94;

#endif
