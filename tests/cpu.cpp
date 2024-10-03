#include <gtest/gtest.h>

#include "core.h"
#include "test.h"

TEST(ADC, Immediate) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // ADC #$ff
    cpu.load_program({0x69, 0xff});
    cpu.A = 0x10;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.A == 0xf);
  HK_TEST(cpu.C == true);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

TEST(ADC, ZeroPage) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // ADC $ab
    cpu.load_program({0x65, 0xab});
    cpu.A = 0x10;
    cpu.write(0x00ab, 0x64);
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.A == 0x74);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

TEST(ADC, ZeroPageX) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // ADC $ab, X
    cpu.load_program({0x75, 0xab});
    cpu.X = 0x11;
    cpu.A = 0x10;
    cpu.write(0x00ab + 0x11, 0x64);
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.A == 0x74);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

TEST(ADC, Absolute) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // ADC $abcd
    cpu.load_program({0x6d, 0xcd, 0xab});
    cpu.A = 0x10;
    cpu.write(0xabcd, 0x64);
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.A == 0x74);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

TEST(ADC, AbsoluteX) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // ADC $abcd, X
    cpu.load_program({0x7d, 0xcd, 0xab});
    cpu.X = 0x11;
    cpu.A = 0x12;
    cpu.write(0xabcd + 0x11, 0x64);
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.A == 0x76);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

TEST(ADC, AbsoluteY) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // ADC $abcd, Y
    cpu.load_program({0x79, 0xcd, 0xab});
    cpu.Y = 0x11;
    cpu.A = 0x12;
    cpu.write(0xabcd + 0x11, 0x64);
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.A == 0x76);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

TEST(ADC, IndirectX) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // ADC ($40, X)
    cpu.load_program({0x61, 0x40});
    cpu.X = 0x11;
    cpu.A = 0x12;
    cpu.write16(0x40 + 0x11, 0xf339);
    cpu.write(0xf339, 0xf3);
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.A == 0x05);
  HK_TEST(cpu.C == true);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

TEST(ADC, IndirectY) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // ADC ($40), Y
    cpu.load_program({0x71, 0x40});
    cpu.Y = 0x11;
    cpu.A = 0x12;
    cpu.write16(0x40, 0xf339);
    cpu.write(0xf339 + 0x11, 0xf3);
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.A == 0x05);
  HK_TEST(cpu.C == true);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

TEST(AND, Immediate) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // AND #$8c
    cpu.load_program({0x29, 0x8c});
    cpu.A = 0xf9;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.A == 0x88);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == true);
}

TEST(ASL, Implicit) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // ASL
    cpu.load_program({0x0a});
    cpu.A = 0xf9;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.A == 0xf2);
  HK_TEST(cpu.C == true);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == true);
}

TEST(BCC, Relative) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;

    //   BCC label
    //   ADC #$11
    // label:
    //   ADC #$05
    cpu.load_program({0x90, 0x02, 0x69, 0x11, 0x69, 0x05});
    cpu.A = 0xf9;
    cpu.exec_n(2);
    return cpu;
  }();

  HK_TEST(cpu.A == 0xfe);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == true);
}

TEST(BCS, Relative) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;

    //   BCS label
    //   ADC #$11
    // label:
    //   ADC #$05
    cpu.load_program({0xb0, 0x02, 0x69, 0x11, 0x69, 0x05});
    cpu.C = true;
    cpu.A = 0xf9;
    cpu.exec_n(2);
    return cpu;
  }();

  HK_TEST(cpu.A == 0xff);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == true);
}

TEST(BEQ, Relative) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;

    //   BEQ label
    //   ADC #$11
    // label:
    //   ADC #$05
    cpu.load_program({0xf0, 0x02, 0x69, 0x11, 0x69, 0x05});
    cpu.Z = true;
    cpu.A = 0xf9;
    cpu.exec_n(2);
    return cpu;
  }();

  HK_TEST(cpu.A == 0xfe);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == true);
}

TEST(BIT, ZeroPage) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // BIT $ab
    cpu.load_program({0x24, 0xab});
    cpu.A = 0x55;
    cpu.write(0x00ab, 0xaa);
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.A == 0x55);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == true);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == true);
}

TEST(BMI, Relative) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    //   BMI label
    //   ADC #$11
    // label:
    //   ADC #$05
    cpu.load_program({0x30, 0x02, 0x69, 0x11, 0x69, 0x05});
    cpu.N = true;
    cpu.A = 0xf9;
    cpu.exec_n(2);
    return cpu;
  }();

  HK_TEST(cpu.A == 0xfe);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == true);
}

TEST(BNE, Relative) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    //   BNE label
    //   ADC #$11
    // label:
    //   ADC #$05
    cpu.load_program({0xd0, 0x02, 0x69, 0x11, 0x69, 0x05});
    cpu.A = 0xf9;
    cpu.exec_n(2);
    return cpu;
  }();

  HK_TEST(cpu.A == 0xfe);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == true);
}

TEST(BPL, Relative) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    //   BPL label
    //   ADC #$11
    // label:
    //   ADC #$05
    cpu.load_program({0x10, 0x02, 0x69, 0x11, 0x69, 0x05});
    cpu.A = 0xf9;
    cpu.exec_n(2);
    return cpu;
  }();

  HK_TEST(cpu.A == 0xfe);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == true);
}

// TODO: BRK

TEST(BVC, Relative) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    //   BVC label
    //   ADC #$11
    // label:
    //   ADC #$05
    cpu.load_program({0x50, 0x02, 0x69, 0x11, 0x69, 0x05});
    cpu.A = 0xf9;
    cpu.exec_n(2);
    return cpu;
  }();

  HK_TEST(cpu.A == 0xfe);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == true);
}

TEST(BVS, Relative) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    //   BVS label
    //   ADC #$11
    // label:
    //   ADC #$05
    cpu.load_program({0x70, 0x02, 0x69, 0x11, 0x69, 0x05});
    cpu.V = true;
    cpu.A = 0xf9;
    cpu.exec_n(2);
    return cpu;
  }();

  HK_TEST(cpu.A == 0xfe);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == true);
}
