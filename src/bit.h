#ifndef CONSTEXPR_6502_BIT_H
#define CONSTEXPR_6502_BIT_H

#include <cstddef>

template <typename T, std::size_t idx>
class bit {
  static_assert(idx < sizeof(T) * 8, "Index out of range...");

  using self = bit<T, idx>;

 public:
  explicit constexpr bit(T& value) : m_value(&value) {}

  constexpr auto get() -> T& { return *m_value; };
  [[nodiscard]] constexpr auto get() const -> T { return *m_value; };

  constexpr auto set() -> void { get() |= m_set_mask; }
  constexpr auto reset() -> void { get() &= m_reset_mask; }

  [[nodiscard]] constexpr auto value() const -> bool {
    return (get() & m_set_mask) >> idx;
  }

  explicit constexpr operator bool() const { return value(); }

  template <typename U>
  constexpr auto operator=(U value) -> self& {
    if (value) {
      set();
    } else {
      reset();
    }

    return *this;
  }

  constexpr auto operator~() const -> bool { return !value(); }

 private:
  T* m_value;

  static constexpr T m_set_mask = (1 << idx);
  static constexpr T m_reset_mask = (1 << idx) ^ 0xff;
};

#endif
