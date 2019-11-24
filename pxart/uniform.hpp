#pragma once
#include <cstddef>
#include <cstdint>
#include <random>
#include <utility>

namespace pxart {

namespace detail {

template <typename Real>
constexpr Real uniform(uint32_t) noexcept = delete;

template <>
constexpr inline float uniform<float>(uint32_t x) noexcept {
  const auto tmp = ((x >> 9) | 0x3f800000);
  return (*reinterpret_cast<const float*>(&tmp)) - 1.0f;
}

template <>
constexpr inline double uniform<double>(uint32_t x) noexcept {
  const auto tmp = ((static_cast<uint64_t>(x) << 20) | 0x3ff0000000000000ULL);
  return (*reinterpret_cast<const double*>(&tmp)) - 1.0;
}

template <typename Real>
constexpr inline Real uniform(uint32_t x, Real a, Real b) noexcept {
  return (b - a) * uniform<Real>(x) + a;
}

template <typename Real>
constexpr Real uniform(uint64_t) noexcept = delete;

template <>
constexpr inline float uniform<float>(uint64_t x) noexcept {
  const auto tmp = (static_cast<uint32_t>(x >> 41) | 0x3f800000);
  return (*reinterpret_cast<const float*>(&tmp)) - 1.0f;
}

template <>
constexpr inline double uniform<double>(uint64_t x) noexcept {
  const auto tmp = ((x >> 12) | 0x3ff0000000000000ULL);
  return (*reinterpret_cast<const double*>(&tmp)) - 1.0;
}

template <>
constexpr inline std::pair<float, float> uniform<std::pair<float, float>>(
    uint64_t x) noexcept {
  return {uniform<float>(static_cast<uint32_t>(x)),
          uniform<float>(static_cast<uint32_t>(x >> 32))};
}

template <typename Real>
constexpr inline Real uniform(uint64_t x, Real a, Real b) noexcept {
  return (b - a) * uniform<Real>(x) + a;
}

}  // namespace detail

template <typename Real, typename RNG>
constexpr inline Real uniform(RNG&& rng) noexcept {
  return detail::uniform<Real>(std::forward<RNG>(rng)());
}

template <typename Real, typename RNG>
constexpr inline Real uniform(RNG&& rng, Real a, Real b) noexcept {
  return detail::uniform(std::forward<RNG>(rng)(), a, b);
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