static inline auto rotate_left(__m256i x, int k) noexcept {
  return _mm256_or_si256(_mm256_slli_epi64(x, k), _mm256_srli_epi64(x, 64 - k));
}

inline auto xoroshiro128plus::operator()() noexcept {
  const auto result = _mm256_add_epi64(s0, s1);
  s1 = _mm256_xor_si256(s0, s1);
  s0 = _mm256_xor_si256(s1, _mm256_xor_si256(_mm256_slli_epi64(s1, shift_b),
                                             rotate_left(s0, rotation_a)));
  s1 = rotate_left(s1, rotation_c);
  return result;
}