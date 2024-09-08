#ifndef CONSTEXPR_6502_COMMON_H
#define CONSTEXPR_6502_COMMON_H

#include <cstdint>
#include <initializer_list>
#include <limits>

using byte = std::uint8_t;
using word = std::uint16_t;
using sbyte = std::int8_t;

using instructions = std::initializer_list<byte>;

template <typename T>
auto to_signed(T value) -> std::make_signed_t<T> {
  using signed_t = std::make_signed_t<T>;
  using limit = std::numeric_limits<signed_t>;

  if (value <= limit::max()) {
    return static_cast<signed_t>(value);
  }

  return static_cast<signed_t>(value - limit::min()) + limit::min();
}

#endif
