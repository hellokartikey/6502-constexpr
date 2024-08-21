#ifndef CONSTEXPR_6502_COMMON_H
#define CONSTEXPR_6502_COMMON_H

#include <cstdint>
#include <initializer_list>

using byte = std::uint8_t;
using word = std::uint16_t;

using instructions = std::initializer_list<byte>;

#endif
