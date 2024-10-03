#ifndef CONSTEXPR_6502_CPU_H
#define CONSTEXPR_6502_CPU_H

#include <array>
#include <functional>
#include <string_view>

#include "core.h"

class cpu6502 {
 public:
  constexpr cpu6502() = default;
  auto reset() -> void;

  constexpr auto exec() -> void {
    auto opcode = fetch();

    U = true;

    (this->*lookup.at(opcode).addrmode)();
    (this->*lookup.at(opcode).operate)();

    U = true;
  }

  constexpr auto exec_n(int value = 1) -> void {
    while (value-- > 0) {
      exec();
    }
  }

  auto exec_all() -> void {
    while (true) {
      exec();
    }
  }

  constexpr auto load_program(instructions program) -> void {
    word addr = 0;
    for (const auto &byte : program) {
      write(addr++, byte);
    }
  }

  constexpr auto fetch() -> byte { return opcode = read(PC++); }

  [[nodiscard]] constexpr auto read(word addr) const -> byte {
    return memory.at(addr);
  }

  [[nodiscard]] constexpr auto read16(word addr) const -> word {
    return static_cast<word>(read(addr + 1) << 8 | read(addr));
  }

  constexpr auto write(word addr, byte data) -> void { memory.at(addr) = data; }

  constexpr auto write16(word addr, word data) -> void {
    memory.at(addr) = static_cast<byte>(data);
    memory.at(addr + 1) = static_cast<byte>(data >> 8);
  }

  // ADDRESSING MODES

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
    auto ptr = fetch();
    address = ((read((ptr + 1) & 0x00ff) << 8) | read(ptr & 0x00ff)) + Y;
  }

  // INSTRUCTIONS

  // Add with Carry
  constexpr void ADC() {
    byte fetched = read(address);
    word temp = A + fetched + static_cast<int>(C);

    C = temp > 255;
    Z = (temp & 0x00FF) == 0;
    V = ((~(A ^ fetched) & (A ^ temp)) & 0x0080) != 0;
    N = (temp & 0x80) != 0;
    A = (temp & 0x00FF);
  }

  // Logical AND
  constexpr void AND() {
    byte fetched = read(address);
    A = A & fetched;
    Z = A == 0x00;
    N = (A & 0x80) != 0;
  }

  // Arithmetic Shift Left
  constexpr void ASL() {
    byte fetched = read(address);
    word temp = static_cast<word>(fetched) << 1;

    C = (temp & 0xFF00) > 0;
    Z = (temp & 0x00FF) == 0x00;
    N = (temp & 0x80) != 0;

    if (lookup.at(opcode).addrmode == &cpu6502::IMP) {
      A = temp & 0x00FF;
    } else {
      write(address, temp & 0x00FF);
    }
  }

  // Branch if Carry Clear
  constexpr void BCC() {
    if (!C) {
      address = PC + address_rel;
      PC = address;
    }
  }

  // Branch if Carry Set
  constexpr void BCS() {
    if (C) {
      address = PC + address_rel;
      PC = address;
    }
  }

  // Branch if Equal
  constexpr void BEQ() {
    if (Z) {
      address = PC + address_rel;
      PC = address;
    }
  }

  // Bit Test
  constexpr void BIT() {
    byte fetched = read(address);
    word temp = A & fetched;
    Z = (temp & 0x00FF) == 0x00;
    N = (fetched & (1 << 7)) != 0;
    V = (fetched & (1 << 6)) != 0;
  }

  // Branch if Minus
  constexpr void BMI() {
    if (N) {
      address = PC + address_rel;
      PC = address;
    }
  }

  // Branch if Not Equal
  constexpr void BNE() {
    if (!Z) {
      address = PC + address_rel;
      PC = address;
    }
  }

  // Branch if Positive
  constexpr void BPL() {
    if (!N) {
      address = PC + address_rel;
      PC = address;
    }
  }

  // Force Interrupt
  constexpr void BRK() {
    PC++;

    I = true;
    write(0x0100 + SP, (PC >> 8) & 0x00FF);
    SP--;
    write(0x0100 + SP, PC & 0x00FF);
    SP--;

    B = true;
    write(0x0100 + SP, getFlag());
    SP--;
    B = false;

    PC = read16(0xfffe);
  }

  // Branch if Overflow Clear
  constexpr void BVC() {
    if (!V) {
      address = PC + address_rel;
      PC = address;
    }
  }

  // Branch if Overflow Set
  constexpr void BVS() {
    if (V) {
      address = PC + address_rel;
      PC = address;
    }
  }

  constexpr void CLC() { C = false; }

  constexpr void CLD() { D = false; }

  constexpr void CLI() { I = false; }

  constexpr void CLV() { V = false; }

  constexpr void CMP() {
    byte fetched = read(address);
    word temp = static_cast<word>(A) - static_cast<word>(fetched);
    C = A >= fetched;
    Z = (temp & 0x00FF) == 0x0000;
    N = (temp & 0x0080) != 0;
  }

  constexpr void CPX() {
    byte fetched = read(address);
    word temp = static_cast<word>(X) - static_cast<word>(fetched);
    C = X >= fetched;
    Z = (temp & 0x00FF) == 0x0000;
    N = (temp & 0x0080) != 0;
  }

  constexpr void CPY() {
    byte fetched = read(address);
    word temp = static_cast<word>(Y) - static_cast<word>(fetched);
    C = Y >= fetched;
    Z = (temp & 0x00FF) == 0x0000;
    N = (temp & 0x0080) != 0;
  }

  constexpr void DEC() {
    byte fetched = read(address);
    word temp = fetched - 1;
    write(address, temp & 0x00FF);
    Z = (temp & 0x00FF) == 0x0000;
    N = (temp & 0x0080) != 0;
  }

  constexpr void DEX() {
    X--;
    Z = X == 0x00;
    N = (X & 0x80) != 0;
  }

  constexpr void DEY() {
    Y--;
    Z = Y == 0x00;
    N = (Y & 0x80) != 0;
  }

  constexpr void EOR() {
    byte fetched = read(address);
    A = A ^ fetched;
    Z = A == 0x00;
    N = (A & 0x80) != 0;
  }

  constexpr void INC() {
    byte fetched = read(address);
    word temp = fetched + 1;
    write(address, temp & 0x00FF);
    Z = (temp & 0x00FF) == 0x0000;
    N = (temp & 0x0080) != 0;
  }

  constexpr void INX() {
    X++;
    Z = X == 0x00;
    N = (X & 0x80) != 0;
  }

  constexpr void INY() {
    Y++;
    Z = Y == 0x00;
    N = (Y & 0x80) != 0;
  }

  // Opcodes

  constexpr void JMP() { PC = address; }

  constexpr void JSR() {
    PC--;
    write16(0x0100 + SP, PC);
    SP -= 2;
    PC = address;
  }

  constexpr void LDA() {
    A = operand;
    Z = A == 0x00;
    N = (A & 0x80) != 0;
  }

  constexpr void LDX() {
    X = operand;
    Z = X == 0x00;
    N = (X & 0x80) != 0;
  }

  constexpr void LDY() {
    Y = operand;
    Z = Y == 0x00;
    N = (Y & 0x80) != 0;
  }

  constexpr void LSR() {
    byte fetched = read(address);
    C = (operand & 0x1) != 0;
    byte temp = fetched >> 1;
    Z = temp == 0x00;
    N = (temp & 0x80) != 0;

    if (lookup.at(opcode).addrmode == &cpu6502::IMP) {
      A = temp;
    } else {
      write(address, temp);
    }
  }

  constexpr void NOP() {}

  constexpr void ORA() {
    A = A | operand;
    Z = A == 0x00;
    N = (A & 0x80) != 0;
  }

  constexpr void PHA() {
    write(0x0100 + SP, A);
    SP--;
  }

  constexpr void PHP() {
    B = true;
    U = true;

    write(0x0100 + SP, getFlag());
    SP--;

    B = false;
    U = false;
  }

  constexpr void PLA() {
    SP++;
    A = read(0x100 + SP);
    Z = A == 0x00;
    N = (A & 0x80) != 0;
  }

  constexpr void PLP() {
    SP++;
    setFlag(read(0x0100 + SP));
    U = true;
  }

  constexpr void ROL() {
    byte temp = (operand << 1) | static_cast<int>(C);
    C = (temp & 0xff00) != 0;
    Z = (temp & 0xff) == 0x00;
    N = (temp & 0x80) != 0;

    if (lookup.at(opcode).addrmode == &cpu6502::IMP) {
      A = temp & 0xff;
    } else {
      write(address, temp & 0xff);
    }
  }

  constexpr void ROR() {
    word temp = (static_cast<int>(C) << 7) | (operand >> 1);
    C = (operand & 0x01) != 0;
    Z = (temp & 0xff) == 0x00;
    N = (temp & 0x80) != 0;

    if (lookup.at(opcode).addrmode == &cpu6502::IMP) {
      A = temp & 0xff;
    } else {
      write(address, temp & 0xff);
    }
  }

  constexpr void RTI() {
    SP++;
    setFlag(read(0x0100 + SP));
    B = false;
    U = false;

    SP++;
    PC = read16(0x0100 + SP);
  }

  constexpr void RTS() {
    SP++;
    PC = read16(0x0100 + SP);

    PC++;
  }

  // Subtract with Carry
  constexpr void SBC() {
    // Operating in 16-bit domain to capture carry out
    // We can invert the bottom 8 bits with bitwise xor
    byte fetched = read(address);
    word value = (fetched) ^ 0x00FF;
    word temp = A + value + static_cast<int>(C);

    C = (temp & 0xFF00) != 0;
    Z = ((temp & 0x00FF) == 0);
    V = ((temp ^ A) & (temp ^ value) & 0x0080) != 0;
    N = (temp & 0x0080) != 0;
    A = (temp & 0x00FF);
  }

  // Set Carry Flag
  constexpr void SEC() { C = true; }

  // Set Decimal Flag
  constexpr void SED() { D = true; }

  // Set Interrupt Disable
  constexpr void SEI() { I = true; }

  // Store Accumulator
  constexpr void STA() { write(address, A); }

  // Store X Register
  constexpr void STX() { write(address, X); }

  // Store Y Register
  constexpr void STY() { write(address, Y); }

  // Transfer Accumulator to X
  constexpr void TAX() {
    X = A;
    Z = X == 0x00;
    N = (X & 0x80) != 0;
  }

  // Transfer Accumulator to Y
  constexpr void TAY() {
    Y = A;
    Z = Y == 0x00;
    N = (Y & 0x80) != 0;
  }

  // Transfer Stack Pointer to X
  constexpr void TSX() {
    X = SP;
    Z = X == 0x00;
    N = (X & 0x80) != 0;
  }

  // Transfer X to Accumulator
  constexpr void TXA() {
    A = X;
    Z = A == 0x00;
    N = (A & 0x80) != 0;
  }

  // Transfer X to Stack Pointer
  constexpr void TXS() { SP = X; }

  // Transfer Y to Accumulator
  constexpr void TYA() {
    A = Y;
    Z = A == 0x00;
    N = (A & 0x80) != 0;
  }

  // Flag stuff
  // https://www.nesdev.org/wiki/Status_flags
  [[nodiscard]] constexpr auto getFlag() const -> byte {
    auto n = static_cast<byte>(N);
    auto v = static_cast<byte>(V);
    auto u = static_cast<byte>(U);
    auto b = static_cast<byte>(B);
    auto d = static_cast<byte>(D);
    auto i = static_cast<byte>(I);
    auto z = static_cast<byte>(Z);
    auto c = static_cast<byte>(C);

    return (n << 7) | (v << 6) | (u << 5) | (b << 4) | (d << 3) | (i << 2) |
           (z << 1) | (c << 0);
  }

  constexpr void setFlag(byte f) {
    N = (f & 0b10000000) != 0;
    V = (f & 0b01000000) != 0;
    U = (f & 0b00100000) != 0;
    B = (f & 0b10010000) != 0;
    D = (f & 0b10001000) != 0;
    I = (f & 0b10000100) != 0;
    Z = (f & 0b10000010) != 0;
    C = (f & 0b10000001) != 0;
  }

  byte operand = 0x00;
  byte opcode = 0x00;
  word address = 0x0000;
  sbyte address_rel = 0x00;

  byte A = 0x00;
  byte X = 0x00;
  byte Y = 0x00;

  word PC = 0x0000;
  byte SP = 0x00;

  bool N = false;
  bool V = false;
  bool U = false;
  bool B = false;
  bool D = false;
  bool I = false;
  bool Z = false;
  bool C = false;

  std::array<byte, 0x10000> memory{};

  struct INSTRUCTION {
    std::string_view name;
    void (cpu6502::*operate)() = nullptr;
    void (cpu6502::*addrmode)() = nullptr;
  };

  using _ = cpu6502;

  // clang-format off
  std::array<INSTRUCTION, 0x100> lookup = {{
		{ "BRK", &_::BRK, &_::IMM },{ "ORA", &_::ORA, &_::IZX },{ "???", &_::NOP, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "ORA", &_::ORA, &_::ZP0 },{ "ASL", &_::ASL, &_::ZP0 },{ "???", &_::NOP, &_::IMP },{ "PHP", &_::PHP, &_::IMP },{ "ORA", &_::ORA, &_::IMM },{ "ASL", &_::ASL, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "ORA", &_::ORA, &_::ABS },{ "ASL", &_::ASL, &_::ABS },{ "???", &_::NOP, &_::IMP },
		{ "BPL", &_::BPL, &_::REL },{ "ORA", &_::ORA, &_::IZY },{ "???", &_::NOP, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "ORA", &_::ORA, &_::ZPX },{ "ASL", &_::ASL, &_::ZPX },{ "???", &_::NOP, &_::IMP },{ "CLC", &_::CLC, &_::IMP },{ "ORA", &_::ORA, &_::ABY },{ "???", &_::NOP, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "ORA", &_::ORA, &_::ABX },{ "ASL", &_::ASL, &_::ABX },{ "???", &_::NOP, &_::IMP },
		{ "JSR", &_::JSR, &_::ABS },{ "AND", &_::AND, &_::IZX },{ "???", &_::NOP, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "BIT", &_::BIT, &_::ZP0 },{ "AND", &_::AND, &_::ZP0 },{ "ROL", &_::ROL, &_::ZP0 },{ "???", &_::NOP, &_::IMP },{ "PLP", &_::PLP, &_::IMP },{ "AND", &_::AND, &_::IMM },{ "ROL", &_::ROL, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "BIT", &_::BIT, &_::ABS },{ "AND", &_::AND, &_::ABS },{ "ROL", &_::ROL, &_::ABS },{ "???", &_::NOP, &_::IMP },
		{ "BMI", &_::BMI, &_::REL },{ "AND", &_::AND, &_::IZY },{ "???", &_::NOP, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "AND", &_::AND, &_::ZPX },{ "ROL", &_::ROL, &_::ZPX },{ "???", &_::NOP, &_::IMP },{ "SEC", &_::SEC, &_::IMP },{ "AND", &_::AND, &_::ABY },{ "???", &_::NOP, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "AND", &_::AND, &_::ABX },{ "ROL", &_::ROL, &_::ABX },{ "???", &_::NOP, &_::IMP },
		{ "RTI", &_::RTI, &_::IMP },{ "EOR", &_::EOR, &_::IZX },{ "???", &_::NOP, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "EOR", &_::EOR, &_::ZP0 },{ "LSR", &_::LSR, &_::ZP0 },{ "???", &_::NOP, &_::IMP },{ "PHA", &_::PHA, &_::IMP },{ "EOR", &_::EOR, &_::IMM },{ "LSR", &_::LSR, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "JMP", &_::JMP, &_::ABS },{ "EOR", &_::EOR, &_::ABS },{ "LSR", &_::LSR, &_::ABS },{ "???", &_::NOP, &_::IMP },
		{ "BVC", &_::BVC, &_::REL },{ "EOR", &_::EOR, &_::IZY },{ "???", &_::NOP, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "EOR", &_::EOR, &_::ZPX },{ "LSR", &_::LSR, &_::ZPX },{ "???", &_::NOP, &_::IMP },{ "CLI", &_::CLI, &_::IMP },{ "EOR", &_::EOR, &_::ABY },{ "???", &_::NOP, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "EOR", &_::EOR, &_::ABX },{ "LSR", &_::LSR, &_::ABX },{ "???", &_::NOP, &_::IMP },
		{ "RTS", &_::RTS, &_::IMP },{ "ADC", &_::ADC, &_::IZX },{ "???", &_::NOP, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "ADC", &_::ADC, &_::ZP0 },{ "ROR", &_::ROR, &_::ZP0 },{ "???", &_::NOP, &_::IMP },{ "PLA", &_::PLA, &_::IMP },{ "ADC", &_::ADC, &_::IMM },{ "ROR", &_::ROR, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "JMP", &_::JMP, &_::IND },{ "ADC", &_::ADC, &_::ABS },{ "ROR", &_::ROR, &_::ABS },{ "???", &_::NOP, &_::IMP },
		{ "BVS", &_::BVS, &_::REL },{ "ADC", &_::ADC, &_::IZY },{ "???", &_::NOP, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "ADC", &_::ADC, &_::ZPX },{ "ROR", &_::ROR, &_::ZPX },{ "???", &_::NOP, &_::IMP },{ "SEI", &_::SEI, &_::IMP },{ "ADC", &_::ADC, &_::ABY },{ "???", &_::NOP, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "ADC", &_::ADC, &_::ABX },{ "ROR", &_::ROR, &_::ABX },{ "???", &_::NOP, &_::IMP },
		{ "???", &_::NOP, &_::IMP },{ "STA", &_::STA, &_::IZX },{ "???", &_::NOP, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "STY", &_::STY, &_::ZP0 },{ "STA", &_::STA, &_::ZP0 },{ "STX", &_::STX, &_::ZP0 },{ "???", &_::NOP, &_::IMP },{ "DEY", &_::DEY, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "TXA", &_::TXA, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "STY", &_::STY, &_::ABS },{ "STA", &_::STA, &_::ABS },{ "STX", &_::STX, &_::ABS },{ "???", &_::NOP, &_::IMP },
		{ "BCC", &_::BCC, &_::REL },{ "STA", &_::STA, &_::IZY },{ "???", &_::NOP, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "STY", &_::STY, &_::ZPX },{ "STA", &_::STA, &_::ZPX },{ "STX", &_::STX, &_::ZPY },{ "???", &_::NOP, &_::IMP },{ "TYA", &_::TYA, &_::IMP },{ "STA", &_::STA, &_::ABY },{ "TXS", &_::TXS, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "STA", &_::STA, &_::ABX },{ "???", &_::NOP, &_::IMP },{ "???", &_::NOP, &_::IMP },
		{ "LDY", &_::LDY, &_::IMM },{ "LDA", &_::LDA, &_::IZX },{ "LDX", &_::LDX, &_::IMM },{ "???", &_::NOP, &_::IMP },{ "LDY", &_::LDY, &_::ZP0 },{ "LDA", &_::LDA, &_::ZP0 },{ "LDX", &_::LDX, &_::ZP0 },{ "???", &_::NOP, &_::IMP },{ "TAY", &_::TAY, &_::IMP },{ "LDA", &_::LDA, &_::IMM },{ "TAX", &_::TAX, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "LDY", &_::LDY, &_::ABS },{ "LDA", &_::LDA, &_::ABS },{ "LDX", &_::LDX, &_::ABS },{ "???", &_::NOP, &_::IMP },
		{ "BCS", &_::BCS, &_::REL },{ "LDA", &_::LDA, &_::IZY },{ "???", &_::NOP, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "LDY", &_::LDY, &_::ZPX },{ "LDA", &_::LDA, &_::ZPX },{ "LDX", &_::LDX, &_::ZPY },{ "???", &_::NOP, &_::IMP },{ "CLV", &_::CLV, &_::IMP },{ "LDA", &_::LDA, &_::ABY },{ "TSX", &_::TSX, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "LDY", &_::LDY, &_::ABX },{ "LDA", &_::LDA, &_::ABX },{ "LDX", &_::LDX, &_::ABY },{ "???", &_::NOP, &_::IMP },
		{ "CPY", &_::CPY, &_::IMM },{ "CMP", &_::CMP, &_::IZX },{ "???", &_::NOP, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "CPY", &_::CPY, &_::ZP0 },{ "CMP", &_::CMP, &_::ZP0 },{ "DEC", &_::DEC, &_::ZP0 },{ "???", &_::NOP, &_::IMP },{ "INY", &_::INY, &_::IMP },{ "CMP", &_::CMP, &_::IMM },{ "DEX", &_::DEX, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "CPY", &_::CPY, &_::ABS },{ "CMP", &_::CMP, &_::ABS },{ "DEC", &_::DEC, &_::ABS },{ "???", &_::NOP, &_::IMP },
		{ "BNE", &_::BNE, &_::REL },{ "CMP", &_::CMP, &_::IZY },{ "???", &_::NOP, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "CMP", &_::CMP, &_::ZPX },{ "DEC", &_::DEC, &_::ZPX },{ "???", &_::NOP, &_::IMP },{ "CLD", &_::CLD, &_::IMP },{ "CMP", &_::CMP, &_::ABY },{ "NOP", &_::NOP, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "CMP", &_::CMP, &_::ABX },{ "DEC", &_::DEC, &_::ABX },{ "???", &_::NOP, &_::IMP },
		{ "CPX", &_::CPX, &_::IMM },{ "SBC", &_::SBC, &_::IZX },{ "???", &_::NOP, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "CPX", &_::CPX, &_::ZP0 },{ "SBC", &_::SBC, &_::ZP0 },{ "INC", &_::INC, &_::ZP0 },{ "???", &_::NOP, &_::IMP },{ "INX", &_::INX, &_::IMP },{ "SBC", &_::SBC, &_::IMM },{ "NOP", &_::NOP, &_::IMP },{ "???", &_::SBC, &_::IMP },{ "CPX", &_::CPX, &_::ABS },{ "SBC", &_::SBC, &_::ABS },{ "INC", &_::INC, &_::ABS },{ "???", &_::NOP, &_::IMP },
		{ "BEQ", &_::BEQ, &_::REL },{ "SBC", &_::SBC, &_::IZY },{ "???", &_::NOP, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "SBC", &_::SBC, &_::ZPX },{ "INC", &_::INC, &_::ZPX },{ "???", &_::NOP, &_::IMP },{ "SED", &_::SED, &_::IMP },{ "SBC", &_::SBC, &_::ABY },{ "NOP", &_::NOP, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "???", &_::NOP, &_::IMP },{ "SBC", &_::SBC, &_::ABX },{ "INC", &_::INC, &_::ABX },{ "???", &_::NOP, &_::IMP },
	}};
  // clang-format on
};

#endif
