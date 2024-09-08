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

  constexpr auto fetch() -> byte { return read(PC++); }

  constexpr auto read(word addr) -> byte { return memory.at(addr); }

  constexpr auto read16(word addr) -> word {
    return static_cast<word>(read(addr + 1) << 8 | read(addr));
  }

  constexpr auto write(word addr, byte data) -> void { memory.at(addr) = data; }

  constexpr auto write16(word addr, word data) -> void {
    memory.at(addr) = static_cast<byte>(data);
    memory.at(addr + 1) = static_cast<byte>(data >> 8);
  }

  constexpr void IMP() { operand = A; }

  constexpr void IMM() { address = PC++; }

  constexpr void ZP0() { address = fetch() & 0x00ff; }

  constexpr void ZPX() { address = (fetch() + X) & 0x00ff; }

  constexpr void ZPY() { address = (fetch() + Y) & 0x00ff; }

  constexpr void REL() { address_rel = to_signed(fetch()); }

  constexpr void ABS() {
    address = read16(PC);
    PC += 2;
  }

  constexpr void ABX() {
    address = read16(PC) + X;
    PC += 2;
  }

  constexpr void ABY() {
    address = read16(PC) + Y;
    PC += 2;
  }

  constexpr void IND() {
    auto ptr = read16(PC);
    PC += 2;

    if ((ptr & 0x00ff) == 0xff) {  // if ptr_lo == 0xff
      address = (read(ptr & 0xff00) << 8) | read(ptr);
    } else {  // expected behaviour
      address = read16(ptr);
    }
  }

  constexpr void IZX() {
    auto ptr = fetch() + X;
    address = (read((ptr + 1) & 0x00ff) << 8) | read(ptr & 0x00ff);
  }

  constexpr void IZY() {
    auto ptr = fetch() + Y;
    address = (read((ptr + 1) & 0x00ff) << 8) | read(ptr & 0x00ff);
  }

 private:
  byte operand = 0x00;
  word address = 0x0000;
  sbyte address_rel = 0x00;

  byte A = 0x00;
  byte X = 0x00;
  byte Y = 0x00;
  byte F = 0x00;

  word PC = 0x0000;
  byte SP = 0x0000;

  bit<byte, 7> N{F};
  bit<byte, 6> V{F};
  bit<byte, 5> U{F};  // Always true
  bit<byte, 4> B{F};
  bit<byte, 3> D{F};
  bit<byte, 2> I{F};
  bit<byte, 1> Z{F};
  bit<byte, 0> C{F};

  std::array<byte, 0x10000> memory{};
};

#endif
