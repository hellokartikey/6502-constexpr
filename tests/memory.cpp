#include <gtest/gtest.h>

#include "core.h"
#include "test.h"

constexpr auto run() {
  cpu6502 emu;

  emu.write(0x1000, 0x64);
  emu.write16(0x2000, 0x1234);

  return emu;
}

TEST(MemoryAPI, ReadByte) {
  constexpr auto cpu = [] {
    cpu6502 emu;

    emu.write(0x1000, 0x64);

    return emu;
  }();

  HK_TEST(cpu.read(0x1000) == 0x64);
}

TEST(MemoryAPI, ReadWord) {
  constexpr auto cpu = run();

  HK_TEST(cpu.read(0x2000) == 0x34);
  HK_TEST(cpu.read(0x2001) == 0x12);
  HK_TEST(cpu.read16(0x2000) == 0x1234);
}

TEST(MemoryAPI, LoadProgram) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    cpu.load_program({0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
                      0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff});
    return cpu;
  }();

  HK_TEST(cpu.read(0x1000) == 0x00);
  HK_TEST(cpu.read(0x1001) == 0x11);
  HK_TEST(cpu.read(0x1002) == 0x22);
  HK_TEST(cpu.read(0x1003) == 0x33);
  HK_TEST(cpu.read(0x1004) == 0x44);
  HK_TEST(cpu.read(0x1005) == 0x55);
  HK_TEST(cpu.read(0x1006) == 0x66);
  HK_TEST(cpu.read(0x1007) == 0x77);
  HK_TEST(cpu.read(0x1008) == 0x88);
  HK_TEST(cpu.read(0x1009) == 0x99);
  HK_TEST(cpu.read(0x100a) == 0xaa);
  HK_TEST(cpu.read(0x100b) == 0xbb);
  HK_TEST(cpu.read(0x100c) == 0xcc);
  HK_TEST(cpu.read(0x100d) == 0xdd);
  HK_TEST(cpu.read(0x100e) == 0xee);
  HK_TEST(cpu.read(0x100f) == 0xff);
}
