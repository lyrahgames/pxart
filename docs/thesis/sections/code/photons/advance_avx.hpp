template <typename RNG>
inline void advance(photons& sys, RNG& rng) noexcept {
  const auto zero = _mm256_setzero_ps();
  const auto one = _mm256_set1_ps(1);
  const auto half = _mm256_set1_ps(0.5);

  const auto time_step = _mm256_set1_ps(sys.time_step);
  const auto scatter = _mm256_set1_ps(sys.scatter);

  for (int i = 0; i < sys.p_x.size(); i += 8) {
    // Load positions and velocities of eight photons.
    const auto p_x = _mm256_loadu_ps(&sys.p_x[i]);
    const auto p_y = _mm256_loadu_ps(&sys.p_y[i]);
    const auto v_x = _mm256_loadu_ps(&sys.v_x[i]);
    const auto v_y = _mm256_loadu_ps(&sys.v_y[i]);
    // Move photons according to their velocity.
    const auto new_p_x = _mm256_add_ps(p_x, _mm256_mul_ps(time_step, v_x));
    const auto new_p_y = _mm256_add_ps(p_y, _mm256_mul_ps(time_step, v_y));
    // Store the new positions.
    _mm256_storeu_ps(&sys.p_x[i], new_p_x);
    _mm256_storeu_ps(&sys.p_y[i], new_p_y);

    // Evaluate which photon has to be scattered.
    const auto rnd = pxart::simd256::uniform<float>(rng);
    const auto mask = _mm256_cmp_ps(rnd, scatter, _CMP_LT_OQ);

    // Sample the Henyey-Greenstein phase function.
    const auto g = _mm256_set1_ps(sys.g);
    const auto sq_g = _mm256_mul_ps(g, g);
    const auto g2 = _mm256_add_ps(g, g);
    auto denom = _mm256_sub_ps(one, g);
    denom = _mm256_add_ps(denom, _mm256_mul_ps(g2, rnd));
    const auto numer = _mm256_sub_ps(one, sq_g);
    const auto frac = _mm256_div_ps(numer, denom);
    auto brackets = _mm256_add_ps(one, sq_g);
    brackets = _mm256_sub_ps(brackets, _mm256_mul_ps(frac, frac));
    const auto cos_angle = _mm256_div_ps(brackets, g2);

    // Sample the sign of the sine of the angle.
    const auto rnd2 = pxart::simd256::uniform<float>(rng);
    const auto abs_sin_angle =
        _mm256_sqrt_ps(_mm256_sub_ps(one, _mm256_mul_ps(cos_angle, cos_angle)));
    const auto lr_mask = _mm256_cmp_ps(rnd2, half, _CMP_LT_OQ);
    const auto sin_angle = _mm256_blendv_ps(
        abs_sin_angle, _mm256_sub_ps(zero, abs_sin_angle), lr_mask);

    // Rotate velocity of photons by matrix-vector product with rotation matrix.
    const auto new_v_x = _mm256_sub_ps(_mm256_mul_ps(cos_angle, v_x),
                                       _mm256_mul_ps(sin_angle, v_y));
    const auto new_v_y = _mm256_add_ps(_mm256_mul_ps(sin_angle, v_x),
                                       _mm256_mul_ps(cos_angle, v_y));

    // Store velocities according to scattering evaluation.
    _mm256_storeu_ps(&sys.v_x[i], _mm256_blendv_ps(new_v_x, v_x, mask));
    _mm256_storeu_ps(&sys.v_y[i], _mm256_blendv_ps(new_v_y, v_y, mask));
  }
}