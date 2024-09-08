#include <fmt/base.h>

#include "core.h"
#include "cpu.h"

auto main() -> int {
  fmt::print("Hello, world!!! :)\n");

  cpu6502 cpu;

  cpu.write(0xabcd, 0x64);
  fmt::print("{:02x}\n", cpu.read(0xabcd));

  cpu.write16(0x0100, 0xabcd);
  fmt::print("{:04x}\n", cpu.read16(0x0100));
  fmt::print("{:02x} {:02x}\n", cpu.read(0x0100), cpu.read(0x0101));

  return 0;
}
