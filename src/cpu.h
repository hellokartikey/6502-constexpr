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

  constexpr auto fetch() -> byte { return read(PC++); }

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

    // TODO: Uncomment the following lines after implementing lookup table.
    // if (lookup[fetch()].addrmode == &IMP)
    //   A = temp & 0x00FF;
    // else
    //   write(address, temp & 0x00FF);
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
    C = operand & 0x1;
    byte temp = fetched >> 1;
    Z = temp == 0x00;
    N = temp & 0x80;

    if (lookup[opcode].addr == &cpu6502::IMP) {
      A = temp;
    } else {
      write(address, temp);
    }
  }

  constexpr void NOP() { return; }

  constexpr void ORA() {
    A = A | operand;
    Z = A == 0x00;
    N = A  & 0x80;
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

  constepxr void PLA() {
    SP++;
    A = read(0x100 + SP);
    Z = A == 0x00;
    N = A & 0x80;
  }

  constepxr void PLP() {
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
    N = (temp & 0x80;

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

 private:
  byte operand = 0x00;
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

  /*struct INSTRUCTION {*/
  /*  std::string_view name;*/
  /*  std::function<void(void)> operate;*/
  /*  std::function<void(void)> addrmode;*/
  /*};*/

  // clang-format off
  // TODO: Uncomment the following lines after implementing all functions of lookup table. 
  /** 
  std::array<INSTRUCTION, 0x100> lookup = {
		{ "BRK", BRK, IMM },{ "ORA", ORA, IZX },{ "???", XXX, IMP },{ "???", XXX, IMP },{ "???", NOP, IMP },{ "ORA", ORA, ZP0 },{ "ASL", ASL, ZP0 },{ "???", XXX, IMP },{ "PHP", PHP, IMP },{ "ORA", ORA, IMM },{ "ASL", ASL, IMP },{ "???", XXX, IMP },{ "???", NOP, IMP },{ "ORA", ORA, ABS },{ "ASL", ASL, ABS },{ "???", XXX, IMP },
		{ "BPL", BPL, REL },{ "ORA", ORA, IZY },{ "???", XXX, IMP },{ "???", XXX, IMP },{ "???", NOP, IMP },{ "ORA", ORA, ZPX },{ "ASL", ASL, ZPX },{ "???", XXX, IMP },{ "CLC", CLC, IMP },{ "ORA", ORA, ABY },{ "???", NOP, IMP },{ "???", XXX, IMP },{ "???", NOP, IMP },{ "ORA", ORA, ABX },{ "ASL", ASL, ABX },{ "???", XXX, IMP },
		{ "JSR", JSR, ABS },{ "AND", AND, IZX },{ "???", XXX, IMP },{ "???", XXX, IMP },{ "BIT", BIT, ZP0 },{ "AND", AND, ZP0 },{ "ROL", ROL, ZP0 },{ "???", XXX, IMP },{ "PLP", PLP, IMP },{ "AND", AND, IMM },{ "ROL", ROL, IMP },{ "???", XXX, IMP },{ "BIT", BIT, ABS },{ "AND", AND, ABS },{ "ROL", ROL, ABS },{ "???", XXX, IMP },
		{ "BMI", BMI, REL },{ "AND", AND, IZY },{ "???", XXX, IMP },{ "???", XXX, IMP },{ "???", NOP, IMP },{ "AND", AND, ZPX },{ "ROL", ROL, ZPX },{ "???", XXX, IMP },{ "SEC", SEC, IMP },{ "AND", AND, ABY },{ "???", NOP, IMP },{ "???", XXX, IMP },{ "???", NOP, IMP },{ "AND", AND, ABX },{ "ROL", ROL, ABX },{ "???", XXX, IMP },
		{ "RTI", RTI, IMP },{ "EOR", EOR, IZX },{ "???", XXX, IMP },{ "???", XXX, IMP },{ "???", NOP, IMP },{ "EOR", EOR, ZP0 },{ "LSR", LSR, ZP0 },{ "???", XXX, IMP },{ "PHA", PHA, IMP },{ "EOR", EOR, IMM },{ "LSR", LSR, IMP },{ "???", XXX, IMP },{ "JMP", JMP, ABS },{ "EOR", EOR, ABS },{ "LSR", LSR, ABS },{ "???", XXX, IMP },
		{ "BVC", BVC, REL },{ "EOR", EOR, IZY },{ "???", XXX, IMP },{ "???", XXX, IMP },{ "???", NOP, IMP },{ "EOR", EOR, ZPX },{ "LSR", LSR, ZPX },{ "???", XXX, IMP },{ "CLI", CLI, IMP },{ "EOR", EOR, ABY },{ "???", NOP, IMP },{ "???", XXX, IMP },{ "???", NOP, IMP },{ "EOR", EOR, ABX },{ "LSR", LSR, ABX },{ "???", XXX, IMP },
		{ "RTS", RTS, IMP },{ "ADC", ADC, IZX },{ "???", XXX, IMP },{ "???", XXX, IMP },{ "???", NOP, IMP },{ "ADC", ADC, ZP0 },{ "ROR", ROR, ZP0 },{ "???", XXX, IMP },{ "PLA", PLA, IMP },{ "ADC", ADC, IMM },{ "ROR", ROR, IMP },{ "???", XXX, IMP },{ "JMP", JMP, IND },{ "ADC", ADC, ABS },{ "ROR", ROR, ABS },{ "???", XXX, IMP },
		{ "BVS", BVS, REL },{ "ADC", ADC, IZY },{ "???", XXX, IMP },{ "???", XXX, IMP },{ "???", NOP, IMP },{ "ADC", ADC, ZPX },{ "ROR", ROR, ZPX },{ "???", XXX, IMP },{ "SEI", SEI, IMP },{ "ADC", ADC, ABY },{ "???", NOP, IMP },{ "???", XXX, IMP },{ "???", NOP, IMP },{ "ADC", ADC, ABX },{ "ROR", ROR, ABX },{ "???", XXX, IMP },
		{ "???", NOP, IMP },{ "STA", STA, IZX },{ "???", NOP, IMP },{ "???", XXX, IMP },{ "STY", STY, ZP0 },{ "STA", STA, ZP0 },{ "STX", STX, ZP0 },{ "???", XXX, IMP },{ "DEY", DEY, IMP },{ "???", NOP, IMP },{ "TXA", TXA, IMP },{ "???", XXX, IMP },{ "STY", STY, ABS },{ "STA", STA, ABS },{ "STX", STX, ABS },{ "???", XXX, IMP },
		{ "BCC", BCC, REL },{ "STA", STA, IZY },{ "???", XXX, IMP },{ "???", XXX, IMP },{ "STY", STY, ZPX },{ "STA", STA, ZPX },{ "STX", STX, ZPY },{ "???", XXX, IMP },{ "TYA", TYA, IMP },{ "STA", STA, ABY },{ "TXS", TXS, IMP },{ "???", XXX, IMP },{ "???", NOP, IMP },{ "STA", STA, ABX },{ "???", XXX, IMP },{ "???", XXX, IMP },
		{ "LDY", LDY, IMM },{ "LDA", LDA, IZX },{ "LDX", LDX, IMM },{ "???", XXX, IMP },{ "LDY", LDY, ZP0 },{ "LDA", LDA, ZP0 },{ "LDX", LDX, ZP0 },{ "???", XXX, IMP },{ "TAY", TAY, IMP },{ "LDA", LDA, IMM },{ "TAX", TAX, IMP },{ "???", XXX, IMP },{ "LDY", LDY, ABS },{ "LDA", LDA, ABS },{ "LDX", LDX, ABS },{ "???", XXX, IMP },
		{ "BCS", BCS, REL },{ "LDA", LDA, IZY },{ "???", XXX, IMP },{ "???", XXX, IMP },{ "LDY", LDY, ZPX },{ "LDA", LDA, ZPX },{ "LDX", LDX, ZPY },{ "???", XXX, IMP },{ "CLV", CLV, IMP },{ "LDA", LDA, ABY },{ "TSX", TSX, IMP },{ "???", XXX, IMP },{ "LDY", LDY, ABX },{ "LDA", LDA, ABX },{ "LDX", LDX, ABY },{ "???", XXX, IMP },
		{ "CPY", CPY, IMM },{ "CMP", CMP, IZX },{ "???", NOP, IMP },{ "???", XXX, IMP },{ "CPY", CPY, ZP0 },{ "CMP", CMP, ZP0 },{ "DEC", DEC, ZP0 },{ "???", XXX, IMP },{ "INY", INY, IMP },{ "CMP", CMP, IMM },{ "DEX", DEX, IMP },{ "???", XXX, IMP },{ "CPY", CPY, ABS },{ "CMP", CMP, ABS },{ "DEC", DEC, ABS },{ "???", XXX, IMP },
		{ "BNE", BNE, REL },{ "CMP", CMP, IZY },{ "???", XXX, IMP },{ "???", XXX, IMP },{ "???", NOP, IMP },{ "CMP", CMP, ZPX },{ "DEC", DEC, ZPX },{ "???", XXX, IMP },{ "CLD", CLD, IMP },{ "CMP", CMP, ABY },{ "NOP", NOP, IMP },{ "???", XXX, IMP },{ "???", NOP, IMP },{ "CMP", CMP, ABX },{ "DEC", DEC, ABX },{ "???", XXX, IMP },
		{ "CPX", CPX, IMM },{ "SBC", SBC, IZX },{ "???", NOP, IMP },{ "???", XXX, IMP },{ "CPX", CPX, ZP0 },{ "SBC", SBC, ZP0 },{ "INC", INC, ZP0 },{ "???", XXX, IMP },{ "INX", INX, IMP },{ "SBC", SBC, IMM },{ "NOP", NOP, IMP },{ "???", SBC, IMP },{ "CPX", CPX, ABS },{ "SBC", SBC, ABS },{ "INC", INC, ABS },{ "???", XXX, IMP },
		{ "BEQ", BEQ, REL },{ "SBC", SBC, IZY },{ "???", XXX, IMP },{ "???", XXX, IMP },{ "???", NOP, IMP },{ "SBC", SBC, ZPX },{ "INC", INC, ZPX },{ "???", XXX, IMP },{ "SED", SED, IMP },{ "SBC", SBC, ABY },{ "NOP", NOP, IMP },{ "???", XXX, IMP },{ "???", NOP, IMP },{ "SBC", SBC, ABX },{ "INC", INC, ABX },{ "???", XXX, IMP },
	};
  */
  // clang-format on
};

#endif
