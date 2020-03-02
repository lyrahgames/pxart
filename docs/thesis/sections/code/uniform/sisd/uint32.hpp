template <typename Real>
constexpr Real uniform(uint32_t) noexcept = delete;

template <>
constexpr inline float uniform<float>(uint32_t x) noexcept {
  const auto tmp = ((x >> 9) | 0x3f800000);
  return (*reinterpret_cast<const float*>(&tmp)) - 1.0f;
}

template <typename Real>
constexpr Real uniform(uint64_t) noexcept = delete;

template <>
constexpr inline double uniform<double>(uint64_t x) noexcept {
  const auto tmp = ((x >> 12) | 0x3ff0000000000000ULL);
  return (*reinterpret_cast<const double*>(&tmp)) - 1.0;
}