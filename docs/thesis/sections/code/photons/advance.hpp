template <typename RNG>
inline void advance(system& sys, RNG& rng) noexcept {
  constexpr float two_pi = 6.283185307;
  constexpr auto square = [](auto x) { return x * x; };
  std::uniform_real_distribution<system::real_type> dist{0, 1};

  const float absorption = 0.1;
  const auto time = 1e-1f;
  const float g = 0.8f;

  for (int i = 0; i < sys.size(); ++i) {
    sys.pos_x[i] += time * sys.v_x[i];
    sys.pos_y[i] += time * sys.v_y[i];

    // const float scattering = 1.0;
    if (pxart::uniform<float>(rng) < std::exp(-absorption * time)) continue;
    // if (pxart::uniform<float>(rng) > scattering) {
    //   sys.weights[i] = 0;
    //   continue;
    // }
    const auto cos_angle =
        (1 + square(g) -
         square((1 - g * g) / (1 - g + 2 * g * pxart::uniform<float>(rng)))) /
        (2 * g);
    const auto sin_angle = ((pxart::uniform<float>(rng) > 0.5) ? 1 : -1) *
                           std::sqrt(1 - square(cos_angle));
    const auto new_x = cos_angle * sys.v_x[i] - sin_angle * sys.v_y[i];
    const auto new_y = sin_angle * sys.v_x[i] + cos_angle * sys.v_y[i];
    sys.v_x[i] = new_x;
    sys.v_y[i] = new_y;
  }
}