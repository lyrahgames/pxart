struct benchmark {
  explicit benchmark(size_t samples);

  template <typename result_type, typename RNG>
  benchmark& run(const char* name, RNG&& rng) noexcept;
  template <typename RNG>
  benchmark& run(const char* name, RNG&& rng) noexcept {
    return run<decltype(rng())>(name, forward<RNG>(rng));
  }
  template <typename result_type, typename RNG>
  benchmark& run(const char* name, RNG&& rng1, RNG&& rng2) noexcept;
  template <typename RNG>
  benchmark& run(const char* name, RNG&& rng1, RNG&& rng2) noexcept {
    return run<decltype(rng1())>(name, forward<RNG>(rng1), forward<RNG>(rng2));
  }
  benchmark& separate() noexcept;

  // Member Variables
  static constexpr int cache_size = 1 << 14;
  size_t n{};
  BenchmarkParameters params{};
  bool header = true;
};