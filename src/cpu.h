#ifndef CONSTEXPR_6502_CPU_H
#define CONSTEXPR_6502_CPU_H

#include <array>

#include "bit.h"

class cpu6502 {
 public:
  auto reset() -> void;
  auto exec() -> void;
  auto exec_n(int value = 1) -> void;
  auto exec_all() -> void;

  auto load_program(instructions program) -> void;

  auto fetch() -> byte;

  auto read(word addr) -> byte;
  auto read16(word addr) -> word;

  auto write(word addr, byte data) -> void;
  auto write16(word addr, word data) -> void;

 private:
  byte A = 0x00;
  byte X = 0x00;
  byte Y = 0x00;
  byte F = 0x00;

  word PC = 0x0000;
  word SP = 0x0000;

  bit<byte, 7> N{F};
  bit<byte, 6> V{F};
  // Always true
  bit<byte, 4> B{F};
  bit<byte, 3> D{F};
  bit<byte, 2> I{F};
  bit<byte, 1> Z{F};
  bit<byte, 0> C{F};

  std::array<byte, 0x10000> memory{};
};

#endif
