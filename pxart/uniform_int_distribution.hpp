#pragma once
#include <immintrin.h>

namespace pxart {

inline auto uniform_int(uint32_t x, uint32_t s) noexcept {
  return static_cast<uint32_t>(
      (static_cast<uint64_t>(x) * static_cast<uint64_t>(s)) >> 32);
}

inline auto uniform_int(__m256i x, uint32_t s) noexcept {
  const auto sv = _mm256_set1_epi32(s);
  const auto x1 = _mm256_srli_epi32(x, 16);
  const auto x0 = _mm256_and_si256(x, _mm256_set1_epi32(0x0000ffff));
  const auto s1 = _mm256_srli_epi32(sv, 16);
  const auto s0 = _mm256_and_si256(sv, _mm256_set1_epi32(0x0000ffff));

  const auto x1s1 = _mm256_mullo_epi32(x1, s1);
  const auto x1s0 = _mm256_mullo_epi32(x1, s0);
  const auto x0s1 = _mm256_mullo_epi32(x0, s1);
  const auto x0s0 = _mm256_mullo_epi32(x0, s0);
  const auto x1s0_0 = _mm256_and_si256(x1s0, _mm256_set1_epi32(0x0000ffff));
  const auto x1s0_1 = _mm256_srli_epi32(x1s0, 16);
  const auto x0s1_0 = _mm256_and_si256(x0s1, _mm256_set1_epi32(0x0000ffff));
  const auto x0s1_1 = _mm256_srli_epi32(x0s1, 16);
  return _mm256_add_epi32(
      x1s1,
      _mm256_add_epi32(
          _mm256_add_epi32(x1s0_1, x0s1_1),
          _mm256_srli_epi32(_mm256_add_epi32(_mm256_add_epi32(x1s0_0, x0s1_0),
                                             _mm256_srli_epi32(x0s0, 16)),
                            16)));
}

}  // namespace pxart