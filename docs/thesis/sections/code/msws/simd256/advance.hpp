inline auto msws::_mm256_square_epi64(simd_type x) noexcept -> simd_type {
  // x = x1 * 2^32 + x_0
  // x^2 = 2 * x_1 * x_2 * 2^32 + x_0^2
  const auto first = _mm256_mul_epu32(x, x);
  const auto second = _mm256_mullo_epi32(x, _mm256_slli_epi64(x, 33));
  return _mm256_add_epi64(first, second);
}

inline auto msws::operator()() noexcept -> simd_type {
  root[0] = _mm256_square_epi64(root[0]);
  root[1] = _mm256_square_epi64(root[1]);
  weyl[0] = _mm256_add_epi64(weyl[0], step[0]);
  weyl[1] = _mm256_add_epi64(weyl[1], step[1]);
  root[0] = _mm256_add_epi64(root[0], weyl[0]);
  root[1] = _mm256_add_epi64(root[1], weyl[1]);
  root[0] = _mm256_or_si256(_mm256_srli_epi64(root[0], 32),
                            _mm256_slli_epi64(root[0], 32));
  root[1] = _mm256_or_si256(_mm256_srli_epi64(root[1], 32),
                            _mm256_slli_epi64(root[1], 32));
  // Interleaved Merging
  return _mm256_blend_epi32(
      _mm256_permutevar8x32_epi32(root[0],
                                  _mm256_set_epi32(7, 5, 3, 1, 6, 4, 2, 0)),
      _mm256_permutevar8x32_epi32(root[1],
                                  _mm256_set_epi32(6, 4, 2, 0, 7, 5, 3, 1)),
      0b11110000);
  // Sequential Merging
  // return _mm256_or_si256(
  //     _mm256_and_si256(root[0], _mm256_set1_epi64x(0xffffffff)),
  //     _mm256_slli_epi64(root[1], 32));
}
