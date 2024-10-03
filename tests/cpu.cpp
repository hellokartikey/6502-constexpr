#include <gtest/gtest.h>

#include "core.h"
#include "test.h"

TEST(ADC, Immediate) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    cpu.load_program({0x69, 0xff});
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.A == 0xff);
}

TEST(ADC, ZeroPage) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    cpu.load_program({0x69, 0xff});
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.A == 0xff);
}
