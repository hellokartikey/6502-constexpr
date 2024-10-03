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

// Clear Carry Flag
TEST(CLC, Implied_Set) {
  // Testing when C=1
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // CLC
    cpu.load_program({0x18});
    cpu.C = true;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

// Clear Carry Flag
TEST(CLC, Implied_Unset) {
  // Testing when C=0
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // CLC
    cpu.load_program({0x18});
    cpu.C = false;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

// Clear Decimal Mode
TEST(CLD, Implied_Set) {
  // Testing when D=1
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // CLD
    cpu.load_program({0xd8});
    cpu.D = true;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

// Clear Decimal Mode
TEST(CLD, Implied_Unset) {
  // Testing when D=0
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // CLD
    cpu.load_program({0xd8});
    cpu.D = false;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

// Clear Interrupt Disable
TEST(CLI, Implied_Set) {
  // Testing when I=1
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // CLI
    cpu.load_program({0x58});
    cpu.I = true;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

// Clear Interrupt Disable
TEST(CLI, Implied_Unset) {
  // Testing when I=0
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // CLI
    cpu.load_program({0x58});
    cpu.I = false;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

// Clear Overflow Flag
TEST(CLV, Implied_Set) {
  // Testing when V=1
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // CLV
    cpu.load_program({0xb8});
    cpu.V = true;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

// Clear Overflow Flag
TEST(CLV, Implied_Unset) {
  // Testing when V=0
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // CLV
    cpu.load_program({0xb8});
    cpu.V = false;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

// Compare
TEST(CMP, Greater) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // CMP #$05
    cpu.load_program({0xc9, 0x05});
    cpu.A = 0x10;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.A == 0x10);
  HK_TEST(cpu.C == true);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

// Compare
TEST(CMP, Less) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // CMP #$1f
    cpu.load_program({0xc9, 0x1f});
    cpu.A = 0x10;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.A == 0x10);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == true);
}

// Compare
TEST(CMP, Equal) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // CMP #$10
    cpu.load_program({0xc9, 0x10});
    cpu.A = 0x10;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.A == 0x10);
  HK_TEST(cpu.C == true);
  HK_TEST(cpu.Z == true);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

// Compare X Register
TEST(CPX, Greater) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // CPX #$05
    cpu.load_program({0xe0, 0x05});
    cpu.X = 0x10;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.X == 0x10);
  HK_TEST(cpu.C == true);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

// Compare X Register
TEST(CPX, Less) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // CPX #$1f
    cpu.load_program({0xe0, 0x1f});
    cpu.X = 0x10;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.X == 0x10);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == true);
}

// Compare X Register
TEST(CPX, Equal) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // CPX #$10
    cpu.load_program({0xe0, 0x10});
    cpu.X = 0x10;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.X == 0x10);
  HK_TEST(cpu.C == true);
  HK_TEST(cpu.Z == true);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

// Compare Y Register
TEST(CPY, Greater) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // CPY #$05
    cpu.load_program({0xc0, 0x05});
    cpu.Y = 0x10;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.Y == 0x10);
  HK_TEST(cpu.C == true);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

// Compare Y Register
TEST(CPY, Less) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // CPY #$1f
    cpu.load_program({0xc0, 0x1f});
    cpu.Y = 0x10;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.Y == 0x10);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == true);
}

// Compare Y Register
TEST(CPY, Equal) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // CPY #$10
    cpu.load_program({0xc0, 0x10});
    cpu.Y = 0x10;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.Y == 0x10);
  HK_TEST(cpu.C == true);
  HK_TEST(cpu.Z == true);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

// Decrement Memory
TEST(DEC, Positive) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // DEC $abcd
    cpu.load_program({0xce, 0xcd, 0xab});
    cpu.write(0xabcd, 0x10);
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.read(0xabcd) == 0x0f);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

// Decrement Memory
TEST(DEC, Zero) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // DEC $abcd
    cpu.load_program({0xce, 0xcd, 0xab});
    cpu.write(0xabcd, 0x01);
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.read(0xabcd) == 0x00);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == true);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

// Decrement Memory
TEST(DEC, Negative) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // DEC $abcd
    cpu.load_program({0xce, 0xcd, 0xab});
    cpu.write(0xabcd, 0x00);
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.read(0xabcd) == 0xff);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == true);
}

// Decrement X Register
TEST(DEX, Positive) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // DEX
    cpu.load_program({0xca});
    cpu.X = 0x10;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.X == 0x0f);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

// Decrement X Register
TEST(DEX, Zero) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // DEX
    cpu.load_program({0xca});
    cpu.X = 0x01;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.X == 0x00);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == true);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

// Decrement X Register
TEST(DEX, Negative) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // DEX
    cpu.load_program({0xca});
    cpu.X = 0x00;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.X == 0xff);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == true);
}

// Decrement Y Register
TEST(DEY, Positive) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // DEY
    cpu.load_program({0x88});
    cpu.Y = 0x10;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.Y == 0x0f);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

// Decrement Y Register
TEST(DEY, Zero) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // DEY
    cpu.load_program({0x88});
    cpu.Y = 0x01;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.Y == 0x00);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == true);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

// Decrement Y Register
TEST(DEY, Negative) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // DEY
    cpu.load_program({0x88});
    cpu.Y = 0x00;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.Y == 0xff);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == true);
}

// Exclusive OR
TEST(EOR, Positive) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // EOR #$55
    cpu.load_program({0x49, 0x55});
    cpu.A = 0x33;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.A == 0x66);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

// Exclusive OR
TEST(EOR, Zero) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // EOR #$55
    cpu.load_program({0x49, 0x55});
    cpu.A = 0x55;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.A == 0x00);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == true);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

// Exclusive OR
TEST(EOR, Negative) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // EOR #$55
    cpu.load_program({0x49, 0x55});
    cpu.A = 0x80;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.A == 0xd5);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == true);
}

// Increment Memory
TEST(INC, Positive) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // INC $abcd
    cpu.load_program({0xee, 0xcd, 0xab});
    cpu.write(0xabcd, 0x10);
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.read(0xabcd) == 0x11);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

// Increment Memory
TEST(INC, Zero) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // INC $abcd
    cpu.load_program({0xee, 0xcd, 0xab});
    cpu.write(0xabcd, 0xff);
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.read(0xabcd) == 0x00);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == true);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

// Increment Memory
TEST(INC, Negative) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // INC $abcd
    cpu.load_program({0xee, 0xcd, 0xab});
    cpu.write(0xabcd, 0x7f);
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.read(0xabcd) == 0x80);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == true);
}

// Increment X Register
TEST(INX, Positive) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // INX
    cpu.load_program({0xe8});
    cpu.X = 0x10;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.X == 0x11);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

// Increment X Register
TEST(INX, Zero) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // INX
    cpu.load_program({0xe8});
    cpu.X = 0xff;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.X == 0x00);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == true);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

// Increment X Register
TEST(INX, Negative) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // INX
    cpu.load_program({0xe8});
    cpu.X = 0x7f;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.X == 0x80);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == true);
}

// Increment Y Register
TEST(INY, Positive) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // INY
    cpu.load_program({0xc8});
    cpu.Y = 0x10;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.Y == 0x11);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

// Increment Y Register
TEST(INY, Zero) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // INY
    cpu.load_program({0xc8});
    cpu.Y = 0xff;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.Y == 0x00);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == true);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

// Increment Y Register
TEST(INY, Negative) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // INY
    cpu.load_program({0xc8});
    cpu.Y = 0x7f;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.Y == 0x80);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == true);
}

// Subtract with Carry
TEST(SBC, Borrowed) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // SBC #$30
    cpu.load_program({0xe9, 0x30});
    cpu.A = 0x50;
    cpu.C = false;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.A == 0x1f);
  HK_TEST(cpu.C == true);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

// Subtract with Carry
TEST(SBC, Positive) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // SBC #$30
    cpu.load_program({0xe9, 0x30});
    cpu.A = 0x50;
    cpu.C = true;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.A == 0x20);
  HK_TEST(cpu.C == true);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

// Subtract with Carry
TEST(SBC, Negative) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // SBC #$50
    cpu.load_program({0xe9, 0x50});
    cpu.A = 0x30;
    cpu.C = true;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.A == 0xe0);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == true);
}

// Subtract with Carry
TEST(SBC, Zero) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // SBC #$30
    cpu.load_program({0xe9, 0x30});
    cpu.A = 0x30;
    cpu.C = true;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.A == 0x00);
  HK_TEST(cpu.C == true);
  HK_TEST(cpu.Z == true);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

// Subtract with Carry
TEST(SBC, Overflow) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // SBC #$01
    cpu.load_program({0xe9, 0x01});
    cpu.A = 0x80;
    cpu.C = true;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.A == 0x7f);
  HK_TEST(cpu.C == true);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == true);
  HK_TEST(cpu.N == false);
}

// Set Carry Flag
TEST(SEC, Implied_Set) {
  // Testing when C=1
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // SEC
    cpu.load_program({0x38});
    cpu.C = true;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.C == true);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

// Set Carry Flag
TEST(SEC, Implied_Unset) {
  // Testing when C=0
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // SEC
    cpu.load_program({0x38});
    cpu.C = false;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.C == true);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

// Set Decimal Flag
TEST(SED, Implied_Set) {
  // Testing when D=1
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // SED
    cpu.load_program({0xf8});
    cpu.D = true;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == true);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

// Set Decimal Flag
TEST(SED, Implied_Unset) {
  // Testing when D=0
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // SED
    cpu.load_program({0xf8});
    cpu.D = false;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == true);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

// Set Interrupt Disable
TEST(SEI, Implied_Set) {
  // Testing when I=1
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // SEI
    cpu.load_program({0x78});
    cpu.I = true;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == true);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

// Set Interrupt Disable
TEST(SEI, Implied_Unset) {
  // Testing when I=0
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // SEI
    cpu.load_program({0x78});
    cpu.I = false;
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == true);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

// Store Accumulator
TEST(STA, Absolute) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // STA $abcd
    cpu.load_program({0x8d, 0xcd, 0xab});
    cpu.A = 0x10;
    cpu.write(0xabcd, 0x00);
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.A == 0x10);
  HK_TEST(cpu.read(0xabcd) == 0x10);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

// Store X Register
TEST(STX, Absolute) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // STX $abcd
    cpu.load_program({0x8e, 0xcd, 0xab});
    cpu.X = 0x10;
    cpu.write(0xabcd, 0x00);
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.X == 0x10);
  HK_TEST(cpu.read(0xabcd) == 0x10);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}

// Store Y Register
TEST(STY, Absolute) {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    // STY $abcd
    cpu.load_program({0x8c, 0xcd, 0xab});
    cpu.Y = 0x10;
    cpu.write(0xabcd, 0x00);
    cpu.exec_n(1);
    return cpu;
  }();

  HK_TEST(cpu.Y == 0x10);
  HK_TEST(cpu.read(0xabcd) == 0x10);
  HK_TEST(cpu.C == false);
  HK_TEST(cpu.Z == false);
  HK_TEST(cpu.I == false);
  HK_TEST(cpu.D == false);
  HK_TEST(cpu.B == false);
  HK_TEST(cpu.V == false);
  HK_TEST(cpu.N == false);
}
