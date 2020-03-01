inline void xoroshiro128plus::jump() noexcept {
  const simd_type jump_mask[] = {_mm256_set1_epi64x(0xdf900294d8f554a5ul),
                                 _mm256_set1_epi64x(0x170865df4b3201fcul)};
  const auto zero = _mm256_setzero_si256();
  const auto one = _mm256_set1_epi64x(1ul);
  auto result0 = zero;
  auto result1 = zero;
  for (int i = 0; i < 2; i++) {
    auto bit = one;
    for (size_t b = 0; b < word_size; ++b) {
      const auto mask =
          _mm256_cmpeq_epi64(_mm256_and_si256(jump_mask[i], bit), zero);
      result0 = _mm256_xor_si256(result0, _mm256_andnot_si256(mask, s0));
      result1 = _mm256_xor_si256(result1, _mm256_andnot_si256(mask, s1));
      s1 = _mm256_xor_si256(s0, s1);
      s0 = _mm256_xor_si256(s1, _mm256_xor_si256(_mm256_slli_epi64(s1, shift_b),
                                                 rotate_left(s0, rotation_a)));
      s1 = rotate_left(s1, rotation_c);
      // operator()();
      bit = _mm256_slli_epi64(bit, 1);
    }
  }
  s0 = result0;
  s1 = result1;
}