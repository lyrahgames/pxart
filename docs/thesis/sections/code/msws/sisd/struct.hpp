struct msws {
  using uint_type = uint64_t;
  using result_type = uint32_t;
  static constexpr size_t word_size = 32;

  constexpr msws() = default;
  msws(const msws&) = default;
  msws& operator=(const msws&) = default;
  msws(msws&&) = default;
  msws& operator=(msws&&) = default;

  template <typename RNG>
  explicit msws(RNG&& rng)
      : s{(static_cast<uint64_t>(rng()) << 32) | (rng() << 1) | 0x01} {}

  constexpr result_type operator()() noexcept;
  static constexpr result_type min() noexcept { return result_type{}; }
  static constexpr result_type max() noexcept { return ~result_type{}; }

  uint_type s = 0xb5ad4eceda1ce2a9;
  uint_type x = 0;
  uint_type w = 0;
};

constexpr auto msws::operator()() noexcept -> result_type {
  x *= x;
  x += (w += s);
  return x = ((x >> 32) | (x << 32));
}