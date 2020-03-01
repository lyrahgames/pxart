#pragma once
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <pxart/utilities/is_valid.hpp>
#include <random>
#include <type_traits>
#include <utility>

namespace pxart {

namespace detail {

// This cast allows to obey strict aliasing rules while reinterpreting between
// floating-point numbers and integers.
template <typename T, typename U>
inline T pun_cast(U x) {
  static_assert(sizeof(T) == sizeof(U));
  T t{};
  std::memcpy(&t, &x, sizeof(x));
  return t;
}

template <typename Real>
constexpr Real uniform(uint32_t) noexcept = delete;

// reinterpret_cast is not allowed in constexpr!
// This would not be portable.
template <>
inline float uniform<float>(uint32_t x) noexcept {
  const auto tmp = ((x >> 9) | 0x3f800000);
  // return (*reinterpret_cast<const float*>(&tmp)) - 1.0f;
  return pun_cast<float>(tmp) - 1.0f;
}

template <>
inline double uniform<double>(uint32_t x) noexcept {
  const auto tmp = ((static_cast<uint64_t>(x) << 20) | 0x3ff0000000000000ULL);
  // return (*reinterpret_cast<const double*>(&tmp)) - 1.0;
  return pun_cast<double>(tmp) - 1.0;
}

template <typename Real,
          std::enable_if_t<std::numeric_limits<Real>::is_iec559, int> = 0>
constexpr inline Real uniform(uint32_t x, Real a, Real b) noexcept {
  return (b - a) * uniform<Real>(x) + a;
}

template <typename Integer,
          std::enable_if_t<std::is_integral_v<Integer> &&
                               (sizeof(Integer) <= sizeof(uint32_t)),
                           int> = 0>
constexpr inline Integer uniform(uint32_t x, Integer a, Integer b) noexcept {
  return a +
         static_cast<Integer>(
             (static_cast<uint64_t>(x) * static_cast<uint64_t>(b - a)) >> 32);
}

template <typename Real>
constexpr Real uniform(uint64_t) noexcept = delete;

// For similar reasons no constexpr is used.
template <>
inline float uniform<float>(uint64_t x) noexcept {
  const auto tmp = (static_cast<uint32_t>(x >> 41) | 0x3f800000);
  // return (*reinterpret_cast<const float*>(&tmp)) - 1.0f;
  return pun_cast<float>(tmp) - 1.0f;
}

template <>
inline double uniform<double>(uint64_t x) noexcept {
  const auto tmp = ((x >> 12) | 0x3ff0000000000000ULL);
  // return (*reinterpret_cast<const double*>(&tmp)) - 1.0;
  return pun_cast<double>(tmp) - 1.0;
}

template <>
inline std::pair<float, float> uniform<std::pair<float, float>>(
    uint64_t x) noexcept {
  return {uniform<float>(static_cast<uint32_t>(x)),
          uniform<float>(static_cast<uint32_t>(x >> 32))};
}

template <typename Real,
          std::enable_if_t<std::numeric_limits<Real>::is_iec559, int> = 0>
constexpr inline Real uniform(uint64_t x, Real a, Real b) noexcept {
  return (b - a) * uniform<Real>(x) + a;
}

template <typename Integer,
          std::enable_if_t<std::is_integral_v<Integer> &&
                               (sizeof(Integer) <= sizeof(uint64_t)),
                           int> = 0>
constexpr inline Integer uniform(uint64_t x, Integer a, Integer b) noexcept {
  const auto m = static_cast<uint64_t>(b - a);
  const auto n = static_cast<uint64_t>(x);
  constexpr auto lower_mask = 0x00000000fffffffful;
  const auto m0 = m & lower_mask;
  const auto m1 = m >> 32;
  const auto n0 = n & lower_mask;
  const auto n1 = n >> 32;
  const auto m0n0 = m0 * n0;
  const auto m0n1 = m0 * n1;
  const auto m1n0 = m1 * n0;
  const auto m1n1 = m1 * n1;
  const auto m0n1_0 = m0n1 & lower_mask;
  const auto m0n1_1 = m0n1 >> 32;
  const auto m1n0_0 = m1n0 & lower_mask;
  const auto m1n0_1 = m1n0 >> 32;
  const auto mul =
      (((m0n0 >> 32) + m1n0_0 + m0n1_0) >> 32) + (m0n1_1 + m1n0_1) + m1n1;
  return a + static_cast<Integer>(mul);
}

}  // namespace detail

constexpr auto has_uniform_01 =
    is_valid([](auto&& x) -> decltype(x.uniform()) {});
constexpr auto has_uniform =
    is_valid([](auto&& x, auto&& y, auto&& z) -> decltype(x.uniform(y, z)) {});

template <typename Real, typename RNG>
constexpr inline auto uniform(RNG&& rng) noexcept
    -> std::enable_if_t<!decltype(has_uniform_01(rng))::value &&
                            std::is_integral_v<decltype(rng())>,
                        Real> {
  return detail::uniform<Real>(std::forward<RNG>(rng)());
}

template <typename Real, typename RNG>
constexpr inline auto uniform(RNG&& rng) noexcept
    -> std::enable_if_t<decltype(has_uniform_01(rng))::value, Real> {
  return std::forward<RNG>(rng).uniform();
}

template <typename Real, typename RNG>
constexpr inline auto uniform(RNG&& rng, Real a, Real b) noexcept
    -> std::enable_if_t<!decltype(has_uniform(rng, a, b))::value, Real> {
  return detail::uniform(std::forward<RNG>(rng)(), a, b);
}

template <typename Real, typename RNG>
constexpr inline auto uniform(RNG&& rng, Real a, Real b) noexcept
    -> std::enable_if_t<decltype(has_uniform(rng, a, b))::value, Real> {
  return std::forward<RNG>(rng).uniform(a, b);
}

template <typename Real>
constexpr inline Real uniform(std::mt19937& rng, Real a = 0,
                              Real b = 1) noexcept {
  return detail::uniform(static_cast<uint32_t>(rng()), a, b);
}

template <typename Real>
constexpr inline Real uniform(std::mt19937&& rng, Real a = 0,
                              Real b = 1) noexcept {
  return detail::uniform(static_cast<uint32_t>(std::move(rng)()), a, b);
}

}  // namespace pxart