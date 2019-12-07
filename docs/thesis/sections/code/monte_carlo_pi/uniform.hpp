template <typename Real, typename Integer, typename RNG>
inline Real monte_carlo_pi(RNG&& rng, Integer samples) noexcept {
  Integer samples_in_circle{};
  for (auto i = samples; i > 0; --i) {
    const auto x = pxart::uniform<Real>(rng);  // Use custom uniform function.
    const auto y = pxart::uniform<Real>(rng);  // The same here.
    samples_in_circle += (x * x + y * y <= 1);
  }
  return static_cast<Real>(samples_in_circle) / samples * 4;
}