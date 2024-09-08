#include <fmt/base.h>

#include "core.h"

auto main() -> int {
  fmt::print("Hello, world!!! :)\n");

  constexpr byte test = 0xff;
  constexpr bit<byte, 0> bit_0(test);

  return 0;
}
