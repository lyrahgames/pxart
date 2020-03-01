#pragma once

#include <cstdint>
#include <type_traits>
#include <utility>

namespace pxart {

template <int Bits, typename Integer>
constexpr inline Integer bit_reversal(Integer x) noexcept;
template <typename Integer>
constexpr inline Integer bit_reversal(Integer x) noexcept;

template <typename RNG>
struct reverse {
  constexpr reverse() = default;
  constexpr explicit reverse(const RNG& r) : rng{r} {}
  constexpr explicit reverse(RNG&& r) : rng{std::move(r)} {}

  constexpr uint32_t operator()() noexcept { return bit_reversal<32>(rng()); }

  RNG rng{};
};

namespace detail {

template <typename Integer>
constexpr auto odd_even_mask = static_cast<Integer>(
    0b0101010101010101010101010101010101010101010101010101010101010101ull);
template <typename Integer>
constexpr auto consecutive_mask = static_cast<Integer>(
    0b0011001100110011001100110011001100110011001100110011001100110011ull);
template <typename Integer>
constexpr auto nibble_mask = static_cast<Integer>(
    0b0000111100001111000011110000111100001111000011110000111100001111ull);
template <typename Integer>
constexpr auto byte_mask = static_cast<Integer>(
    0b0000000011111111000000001111111100000000111111110000000011111111ull);
template <typename Integer>
constexpr auto short_mask = static_cast<Integer>(
    0b0000000000000000111111111111111100000000000000001111111111111111ull);

template <typename Integer, int Bits>
struct bit_reverser;

template <typename Integer>
struct bit_reverser<Integer, 8> {
  constexpr Integer operator()(Integer x) const noexcept {
    x = ((x >> 1) & odd_even_mask<Integer>) |
        ((x & odd_even_mask<Integer>) << 1);
    x = ((x >> 2) & consecutive_mask<Integer>) |
        ((x & consecutive_mask<Integer>) << 2);
    if constexpr (sizeof(Integer) == 1)
      return (x >> 4) | (x << 4);
    else
      return x = ((x >> 4) & nibble_mask<Integer>) |
                 ((x & nibble_mask<Integer>) << 4);
  }
};

template <typename Integer>
struct bit_reverser<Integer, 16> {
  constexpr Integer operator()(Integer x) const noexcept {
    bit_reverser<Integer, 8> reversal;
    x = reversal(x);
    if constexpr (sizeof(Integer) == 2)
      return (x >> 8) | (x << 8);
    else
      return x = ((x >> 8) & byte_mask<Integer>) |
                 ((x & byte_mask<Integer>) << 8);
  }
};

template <typename Integer>
struct bit_reverser<Integer, 32> {
  constexpr Integer operator()(Integer x) const noexcept {
    bit_reverser<Integer, 16> reversal;
    x = reversal(x);
    if constexpr (sizeof(Integer) == 4)
      return (x >> 16) | (x << 16);
    else
      return x = ((x >> 16) & short_mask<Integer>) |
                 ((x & short_mask<Integer>) << 16);
  }
};

template <typename Integer>
struct bit_reverser<Integer, 64> {
  constexpr Integer operator()(Integer x) const noexcept {
    bit_reverser<Integer, 32> reversal;
    x = reversal(x);
    return (x >> 32) | (x << 32);
  }
};

}  // namespace detail

// The implementation of bit_reverser makes only sense for unsigned integers.
// Shift operations for signed are implementation defined and are therefore not
// portable. Instead we use a compile-time cast to an unsigned variant.
template <int Bits, typename Integer>
constexpr inline Integer bit_reversal(Integer x) noexcept {
  return detail::bit_reverser<std::make_unsigned_t<Integer>, Bits>{}(x);
}

template <typename Integer>
constexpr inline Integer bit_reversal(Integer x) noexcept {
  return detail::bit_reverser<std::make_unsigned_t<Integer>,
                              8 * sizeof(Integer)>{}(x);
}

}  // namespace pxart