template <typename Real, typename Integer, typename RNG>
inline Real monte_carlo_pi(RNG&& rng, Integer samples) noexcept {
  constexpr int cache_size = sizeof(decltype(rng())) / sizeof(Real);
  Integer samples_in_circle{};
  for (Integer i = 0; i < samples; i += cache_size) {
    const auto cache_x = pxart::uniform<Real>(rng);
    const auto cache_y = pxart::uniform<Real>(rng);
    for (int j = 0; j < cache_size; ++j) {
      const auto x = reinterpret_cast<const Real*>(&cache_x)[j];
      const auto y = reinterpret_cast<const Real*>(&cache_y)[j];
      samples_in_circle += (x * x + y * y <= 1);
    }
  }
  return static_cast<Real>(samples_in_circle) / samples * 4;
}