#include <iostream>
#include "cpu.h"

int main() {
  constexpr cpu6502 cpu = [] {
    cpu6502 cpu;
    cpu.load_program({0x02});
    cpu.exec_until_hlt();
    return cpu;
  }();
}
