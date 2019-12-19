template <typename RNG>
inline void advance(photons& sys, RNG& rng) noexcept {
  const auto time = 1e-1f;
  const float g = -0.8f;
  const float absorption = 0.1;
  const auto free_time = std::exp(-absorption * time);

  const auto v_time = _mm256_set1_ps(time);
  const auto v_free_time = _mm256_set1_ps(free_time);

  for (int i = 0; i < sys.size(); i += 8) {
    const auto pos_x = _mm256_loadu_ps(&sys.pos_x[i]);
    const auto pos_y = _mm256_loadu_ps(&sys.pos_y[i]);
    const auto v_x = _mm256_loadu_ps(&sys.v_x[i]);
    const auto v_y = _mm256_loadu_ps(&sys.v_y[i]);
    const auto new_pos_x = _mm256_add_ps(pos_x, _mm256_mul_ps(v_time, v_x));
    const auto new_pos_y = _mm256_add_ps(pos_y, _mm256_mul_ps(v_time, v_y));
    // const auto new_pos_x = _mm256_fmadd_ps(v_time, v_x, pos_x);
    // const auto new_pos_y = _mm256_fmadd_ps(v_time, v_y, pos_y);
    _mm256_storeu_ps(&sys.pos_x[i], new_pos_x);
    _mm256_storeu_ps(&sys.pos_y[i], new_pos_y);

    const auto rnd = pxart::simd256::uniform<float>(rng);
    const auto mask = _mm256_cmp_ps(rnd, v_free_time, _CMP_LT_OQ);

    const auto v_g = _mm256_set1_ps(g);
    const auto v_g2 = _mm256_mul_ps(v_g, v_g);
    const auto one = _mm256_set1_ps(1);
    const auto mone = _mm256_set1_ps(-1);
    const auto two = _mm256_set1_ps(2);
    const auto v_2g = _mm256_mul_ps(two, v_g);
    auto denom = _mm256_sub_ps(one, v_g);
    denom = _mm256_add_ps(denom, _mm256_mul_ps(v_2g, rnd));
    const auto numer = _mm256_sub_ps(one, v_g2);
    const auto frac = _mm256_div_ps(numer, denom);
    auto brackets = _mm256_add_ps(one, v_g2);
    brackets = _mm256_sub_ps(brackets, _mm256_mul_ps(frac, frac));
    const auto cos_angle = _mm256_div_ps(brackets, v_2g);

    const auto rnd2 = pxart::simd256::uniform<float>(rng);

    const auto abs_sin_angle =
        _mm256_sqrt_ps(_mm256_sub_ps(one, _mm256_mul_ps(cos_angle, cos_angle)));
    const auto lr_mask = _mm256_cmp_ps(rnd2, _mm256_set1_ps(0.5), _CMP_LT_OQ);
    const auto sin_angle = _mm256_blendv_ps(
        abs_sin_angle, _mm256_mul_ps(mone, abs_sin_angle), lr_mask);

    const auto new_x = _mm256_sub_ps(_mm256_mul_ps(cos_angle, v_x),
                                     _mm256_mul_ps(sin_angle, v_y));
    const auto new_y = _mm256_add_ps(_mm256_mul_ps(sin_angle, v_x),
                                     _mm256_mul_ps(cos_angle, v_y));
    _mm256_storeu_ps(&sys.v_x[i], _mm256_blendv_ps(new_x, v_x, mask));
    _mm256_storeu_ps(&sys.v_y[i], _mm256_blendv_ps(new_y, v_y, mask));
  }
}