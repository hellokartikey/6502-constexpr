#ifndef CONSTEXPR_6502_CPU_H
#define CONSTEXPR_6502_CPU_H

#include <array>
#include <functional>

#include "bit.h"

class cpu6502 {
 public:
  constexpr cpu6502(){};
  auto reset() -> void;
  auto exec() -> void;
  auto exec_n(int value = 1) -> void;
  auto exec_all() -> void;

  auto load_program(instructions program) -> void;

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
    auto ptr = fetch() + Y;
    address = (read((ptr + 1) & 0x00ff) << 8) | read(ptr & 0x00ff);
  }

  // INSTRUCTIONS

  // Add with Carry
  constexpr void ADC() {
    byte fetched = read(address);
    word temp = (word)A + (word)fetched + (word)C;

    C = temp > 255;
    Z = (temp & 0x00FF) == 0;
    V = (~((word)A ^ (word)fetched) & ((word)A ^ (word)temp)) & 0x0080;
    N = temp & 0x80;
    A = temp & 0x00FF;
  }

  // Logical AND
  constexpr void AND() {
    byte fetched = read(address);
    A = A & fetched;
    Z = A == 0x00;
    N = A & 0x80;
  }

  // Arithmetic Shift Left
  constexpr void ASL() {
    byte fetched = read(address);
    word temp = (word)fetched << 1;

    C = (temp & 0xFF00) > 0;
    Z = (temp & 0x00FF) == 0x00;
    N = temp & 0x80;

    if (lookup[opcode].addrmode == &cpu6502::IMP)
      A = temp & 0x00FF;
    else
      write(address, temp & 0x00FF);
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
    N = fetched & (1 << 7);
    V = fetched & (1 << 6);
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

    I = 1;
    write(0x0100 + SP, (PC >> 8) & 0x00FF);
    SP--;
    write(0x0100 + SP, PC & 0x00FF);
    SP--;

    B = 1;
    write(0x0100 + SP, F);
    SP--;
    B = 0;

    PC = (word)read(0xFFFE) | ((word)read(0xFFFF) << 8);
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
    N = temp & 0x0080;
  }

  constexpr void CPX() {
    byte fetched = read(address);
    word temp = static_cast<word>(X) - static_cast<word>(fetched);
    C = X >= fetched;
    Z = (temp & 0x00FF) == 0x0000;
    N = temp & 0x0080;
  }

  constexpr void CPY() {
    byte fetched = read(address);
    word temp = static_cast<word>(Y) - static_cast<word>(fetched);
    C = Y >= fetched;
    Z = (temp & 0x00FF) == 0x0000;
    N = temp & 0x0080;
  }

  constexpr void DEC() {
    byte fetched = read(address);
    word temp = fetched - 1;
    write(address, temp & 0x00FF);
    Z = (temp & 0x00FF) == 0x0000;
    N = temp & 0x0080;
  }

  constexpr void DEX() {
    X--;
    Z = X == 0x00;
    N = X & 0x80;
  }

  constexpr void DEY() {
    Y--;
    Z = Y == 0x00;
    N = Y & 0x80;
  }

  constexpr void EOR() {
    byte fetched = read(address);
    A = A ^ fetched;
    Z = A == 0x00;
    N = A & 0x80;
  }

  constexpr void INC() {
    byte fetched = read(address);
    word temp = fetched + 1;
    write(address, temp & 0x00FF);
    Z = (temp & 0x00FF) == 0x0000;
    N = temp & 0x0080;
  }

  constexpr void INX() {
    X++;
    Z = X == 0x00;
    N = X & 0x80;
  }

  constexpr void INY() {
    Y++;
    Z = Y == 0x00;
    N = Y & 0x80;
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
    N = A & 0x80;
  }

  constexpr void LDX() {
    X = operand;
    Z = X == 0x00;
    N = X & 0x80;
  }

  constexpr void LDY() {
    Y = operand;
    Z = Y == 0x00;
    N = Y & 0x80;
  }

  constexpr void LSR() {
    byte fetched = read(address);
    C = operand & 0x1;
    byte temp = fetched >> 1;
    Z = temp == 0x00;
    N = temp & 0x80;

    if (lookup[opcode].addrmode == &cpu6502::IMP) {
      A = temp;
    } else {
      write(address, temp);
    }
  }

  constexpr void NOP() { return; }

  constexpr void ORA() {
    A = A | operand;
    Z = A == 0x00;
    N = A & 0x80;
  }

  constexpr void PHA() {
    write(0x0100 + SP, A);
    SP--;
  }

  constexpr void PHP() {
    B = 1;
    U = 1;

    write(0x0100 + SP, F);
    SP--;

    B = 0;
    U = 0;
  }

  constexpr void PLA() {
    SP++;
    A = read(0x100 + SP);
    Z = A == 0x00;
    N = A & 0x80;
  }

  constexpr void PLP() {
    SP++;
    F = read(0x0100 + SP);
    U == 1;
  }

  constexpr void ROL() {
    byte temp = (operand << 1) | C;
    C = temp & 0xff00;
    Z = (temp & 0xff) == 0x00;
    N = temp & 0x80;

    if (lookup[opcode].addrmode == &cpu6502::IMP) {
      A = temp & 0xff;
    } else {
      write(address, temp & 0xff);
    }
  }

  constexpr void ROR() {
    word temp = (C << 7) | (operand >> 1);
    C = operand & 0x01;
    Z = (temp & 0xff) == 0x00;
    N = temp & 0x80;

    if (lookup[opcode].addrmode == &cpu6502::IMP) {
      A = temp & 0xff;
    } else {
      write(address, temp & 0xff);
    }
  }

  constexpr void RTI() {
    SP++;
    F = read(0x0100 + SP);
    B = 0;
    U = 0;

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
    word value = ((word)fetched) ^ 0x00FF;
    word temp = (word)A + value + (word)C;

    C = temp & 0xFF00;
    Z = ((temp & 0x00FF) == 0);
    V = (temp ^ (word)A) & (temp ^ value) & 0x0080;
    N = temp & 0x0080;
    A = temp & 0x00FF;
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
    N = X & 0x80;
  }

  // Transfer Accumulator to Y
  constexpr void TAY() {
    Y = A;
    Z = Y == 0x00;
    N = Y & 0x80;
  }

  // Transfer Stack Pointer to X
  constexpr void TSX() {
    X = SP;
    Z = X == 0x00;
    N = X & 0x80;
  }

  // Transfer X to Accumulator
  constexpr void TXA() {
    A = X;
    Z = A == 0x00;
    N = A & 0x80;
  }

  // Transfer X to Stack Pointer
  constexpr void TXS() { SP = X; }

  // Transfer Y to Accumulator
  constexpr void TYA() {
    A = Y;
    Z = A == 0x00;
    N = A & 0x80;
  }

 private:
  byte operand = 0x00;
  byte opcode = 0x00;
  word address = 0x0000;
  sbyte address_rel = 0x00;

  byte A = 0x00;
  byte X = 0x00;
  byte Y = 0x00;

  word PC = 0x0000;
  byte SP = 0x00;

  union {
    byte F = 0x00;
    struct {
      bool N : 1;
      bool V : 1;
      bool U : 1;
      bool B : 1;
      bool D : 1;
      bool I : 1;
      bool Z : 1;
      bool C : 1;
    };
  };

  std::array<byte, 0x10000> memory{};

  struct INSTRUCTION {
    std::string_view name;
    void (cpu6502::*operate)(void) = nullptr;
    void (cpu6502::*addrmode)(void) = nullptr;
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
