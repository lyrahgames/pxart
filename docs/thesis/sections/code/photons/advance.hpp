template <typename RNG>
inline void advance(system& sys, RNG&& rng) noexcept {
  for (int i = 0; i < sys.p_x.size(); ++i) {
    // Move photon according to its velocity.
    sys.p_x[i] += sys.time_step * sys.v_x[i];
    sys.p_y[i] += sys.time_step * sys.v_y[i];

    // Sample if scattering occurs.
    if (pxart::uniform<float>(rng) > sys.scatter) continue;

    // Sample Henyey-Greenstein phase function to scatter.
    const auto g = sys.g;
    const auto tmp = (1 - g * g) / (1 - g + 2 * g * pxart::uniform<float>(rng));
    const auto cos_angle = (1 + g * g - tmp * tmp) / (2 * g);
    // Sample the sign of the sine of the angle.
    const auto sign = (pxart::uniform<float>(rng) > 0.5) ? 1 : -1;
    const auto sin_angle = sign * std::sqrt(1 - cos_angle * cos_angle);

    // Rotate velocity of photon by matrix-vector product with rotation matrix.
    const auto new_x = cos_angle * sys.v_x[i] - sin_angle * sys.v_y[i];
    const auto new_y = sin_angle * sys.v_x[i] + cos_angle * sys.v_y[i];
    sys.v_x[i] = new_x;
    sys.v_y[i] = new_y;
  }
}