inline auto mt19937::operator()() noexcept -> simd_type {
  // Precompute 624 transitions when all elements have been used.
  if (state_index >= state_size) {
    // Transition for Single Element
    const auto transition = [this](int k, int k_shift) constexpr {
      // Constants
      const auto simd_upper_mask = _mm256_set1_epi32(upper_mask);
      const auto simd_lower_mask = _mm256_set1_epi32(lower_mask);
      const auto simd_zero = _mm256_setzero_si256();
      const auto simd_one = _mm256_set1_epi32(1);
      const auto simd_xor_mask = _mm256_set1_epi32(xor_mask);

      // Load elements.
      const auto s0 =
          _mm256_load_si256(reinterpret_cast<const simd_type*>(&state[k]));
      const auto s1 =
          _mm256_loadu_si256(reinterpret_cast<const simd_type*>(&state[k + 1]));
      const auto ss = _mm256_loadu_si256(
          reinterpret_cast<const simd_type*>(&state[k_shift]));

      // Compute transition.
      const auto x = _mm256_or_si256(_mm256_and_si256(s0, simd_upper_mask),
                                     _mm256_and_si256(s1, simd_lower_mask));
      const auto y = _mm256_and_si256(
          simd_xor_mask,
          _mm256_cmpgt_epi32(_mm256_and_si256(x, simd_one), simd_zero));
      const auto z = _mm256_xor_si256(_mm256_srli_epi32(x, 1), y);
      return _mm256_xor_si256(ss, z);
    };

    // AVX Transition Loop
    // 1. Part
    const auto first = transition(0, shift_size);
    _mm256_store_si256(reinterpret_cast<simd_type*>(&state[0]), first);
    _mm256_store_si256(reinterpret_cast<simd_type*>(&state[state_size]), first);
    // 2. Part
    int k = simd_size;
    for (; k < state_size - shift_size; k += simd_size) {
      const auto result = transition(k, k + shift_size);
      _mm256_store_si256(reinterpret_cast<simd_type*>(&state[k]), result);
    }
    // 3. Part
    for (; k < state_size; k += simd_size) {
      const auto result = transition(k, k + shift_size - state_size);
      _mm256_store_si256(reinterpret_cast<simd_type*>(&state[k]), result);
    }

    state_index = 0;
  }

  // Generator Function
  auto x = _mm256_load_si256(
      reinterpret_cast<const simd_type*>(&state[state_index]));
  state_index += simd_size;
  x = _mm256_xor_si256(x, _mm256_srli_epi32(x, tempering_u_shift));
  x = _mm256_xor_si256(x,
                       _mm256_and_si256(_mm256_slli_epi32(x, tempering_s_shift),
                                        _mm256_set1_epi32(tempering_b_mask)));
  x = _mm256_xor_si256(x,
                       _mm256_and_si256(_mm256_slli_epi32(x, tempering_t_shift),
                                        _mm256_set1_epi32(tempering_c_mask)));
  x = _mm256_xor_si256(x, _mm256_srli_epi32(x, tempering_l_shift));
  return x;
}