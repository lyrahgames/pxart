#pragma once
#include <array>
#include <iterator>

namespace pxart {

template <typename T, size_t N>
struct seed_seq {
  using uint_type = T;
  using result_type = uint_type;
  static constexpr size_t state_size = N;
  static constexpr uint_type init_a = 0x43b0d7e5;
  static constexpr uint_type mult_a = 0x931e8875;
  static constexpr uint_type init_b = 0x8b51f9dd;
  static constexpr uint_type mult_b = 0x58f38ded;
  static constexpr uint_type mix_multiplier_l = 0xca01f9dd;
  static constexpr uint_type mix_multiplier_r = 0x4973f715;
  static constexpr size_t xor_shift = sizeof(uint_type) * 8 / 2;

  template <T multiplier>
  struct hasher;
  static constexpr uint_type mix(uint_type x, uint_type y) noexcept;

  template <typename InputIt>
  seed_seq(InputIt begin, InputIt end) noexcept;
  template <typename U>
  seed_seq(std::initializer_list<U> init) noexcept
      : seed_seq(begin(init), end(init)) {}
  constexpr seed_seq() noexcept : seed_seq{1, 2, 3, 4} {}
  seed_seq(const seed_seq&) = default;
  seed_seq& operator=(const seed_seq&) = default;
  seed_seq(seed_seq&&) = default;
  seed_seq& operator=(seed_seq&&) = default;

  constexpr result_type operator()() noexcept;

  std::array<uint_type, state_size> data{};
  uint_type hash_const = init_b;
  hasher<mult_b> hash{init_b};
  size_t index{};
};

template <typename T, size_t N>
template <T multiplier>
struct seed_seq<T, N>::hasher {
  constexpr uint_type operator()(uint_type x) noexcept {
    x ^= state;           // Avoid mapping zero to zero.
    state *= multiplier;  // Generate new pseudorandom multiplier.
    x *= state;           // Do the multiply-
    x ^= x >> xor_shift;  // xorshift.
    return x;
  }

  uint_type state{};
};

template <typename T, size_t N>
constexpr auto seed_seq<T, N>::mix(uint_type x, uint_type y) noexcept
    -> uint_type {
  uint_type result = mix_multiplier_l * x - mix_multiplier_r * y;
  result ^= result >> xor_shift;
  return result;
}

template <typename T, size_t N>
template <typename InputIt>
seed_seq<T, N>::seed_seq(InputIt first, InputIt last) noexcept {
  hasher<mult_a> hash{init_a};

  // Initialize internal state by hash functions.
  size_t i = 0;
  InputIt it = first;
  for (; (i < state_size) && (it != last); ++i, ++it)  //
    data[i] = hash(*it);
  for (; i < state_size; ++i)  //
    data[i] = hash(0);

  // Mix entropy of input.
  for (size_t p = 0; p < state_size; ++p) {
    for (size_t q = 0; q < p; ++q)  //
      data[q] = mix(data[q], hash(data[p]));
    for (size_t q = p + 1; q < state_size; ++q)
      data[q] = mix(data[q], hash(data[p]));
  }

  // Mix extra entropy given in input into state.
  for (; it != last; ++it)
    for (auto& x : data) x = mix(x, hash(*it));
}

template <typename T, size_t N>
constexpr auto seed_seq<T, N>::operator()() noexcept -> result_type {
  if (index == state_size) index = 0;
  return hash(data[index++]);
}

using seed_seq_4x32 = seed_seq<uint32_t, 4>;
using seed_seq_8x32 = seed_seq<uint32_t, 8>;
using seed_seq_4x64 = seed_seq<uint64_t, 4>;

}  // namespace pxart