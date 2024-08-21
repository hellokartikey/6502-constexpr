#ifndef CONSTEXPR_6502_CPU_H
#define CONSTEXPR_6502_CPU_H

#include <array>

class cpu6502 {
 public:
   auto reset() -> void;
   auto exec() -> void;
   auto exec_n(int value = 1) -> void;

   auto load_program(instructions program) -> void;

   auto flag(flags f) -> bool;
   auto flag(flags f, bool value) -> void;

   auto fetch() -> byte;

   auto read(word addr) -> byte;
   auto read16(word addr) -> word;

   auto write(word addr, byte data) -> void;
   auto write16(word addr, word data) -> void;

 private:
   byte A, X, Y, F;
   word PC, SP;

   std::array<byte, 0x10000> memory;
};

#endif
