#include <gtest/gtest.h>

#include "core.h"
#include "test.h"

constexpr auto run() {
  cpu6502 emu;

  emu.write(0x1000, 0x64);
  emu.write16(0x2000, 0x1234);

  return emu;
}

TEST(MemoryAPIFixture, ReadByte) {
  constexpr auto cpu = run();

  HK_TEST(cpu.read(0x1000) == 0x64);
}

TEST(MemoryAPIFixture, ReadWord) {
  constexpr auto cpu = run();

  HK_TEST(cpu.read(0x2000) == 0x34);
  HK_TEST(cpu.read(0x2001) == 0x12);
  HK_TEST(cpu.read16(0x2000) == 0x1234);
}
