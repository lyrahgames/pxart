template <typename Real, typename Integer, typename RNG>
inline Real monte_carlo_pi(RNG&& rng1, RNG&& rng2, Integer samples) noexcept {
  std::uniform_real_distribution<Real> dist{0, 1};
  Integer samples_in_circle{};
  for (auto i = samples; i > 0; --i) {
    const auto x = dist(rng1);  // Use first instance.
    const auto y = dist(rng2);  // Use second instance.
    samples_in_circle += (x * x + y * y <= 1);
  }
  return static_cast<Real>(samples_in_circle) / samples * 4;
}