template <typename Real, typename Integer, typename RNG>
inline Real monte_carlo_pi(RNG& rng, Integer samples) noexcept {
  // std::uniform_real_distribution<Real> dist{0, 1};
  Integer samples_in_circle{};
  for (auto i = samples; i > 0; --i) {
    // const auto x = dist(rng);
    // const auto y = dist(rng);
    const auto x = pxart::uniform<Real>(rng);
    const auto y = pxart::uniform<Real>(rng);
    samples_in_circle += (x * x + y * y <= 1);
  }
  return static_cast<Real>(samples_in_circle) / samples * 4;
}