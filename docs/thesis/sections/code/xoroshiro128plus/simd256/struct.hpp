struct xoroshiro128plus {
  using uint_type = uint64_t;
  using simd_type = __m256i;
  using result_type = uint_type;
  static constexpr size_t simd_size = 4;
  static constexpr size_t word_size = 64;
  static constexpr size_t rotation_a = 24;
  static constexpr size_t shift_b = 16;
  static constexpr size_t rotation_c = 37;

  static inline auto rotate_left(__m256i x, int k) noexcept {
    return _mm256_or_si256(_mm256_slli_epi64(x, k),
                           _mm256_srli_epi64(x, 64 - k));
  }

  xoroshiro128plus() = default;
  xoroshiro128plus(const xoroshiro128plus& rng) = default;
  xoroshiro128plus& operator=(const xoroshiro128plus&) = default;
  xoroshiro128plus(xoroshiro128plus&&) = default;
  xoroshiro128plus& operator=(xoroshiro128plus&&) = default;

  template <typename RNG>
  explicit xoroshiro128plus(RNG&& rng)
      : s0{_mm256_set_epi32(rng(), rng(), rng(), rng(), rng(), rng(), rng(),
                            rng())},
        s1{_mm256_set_epi32(rng(), rng(), rng(), rng(), rng(), rng(), rng(),
                            rng())} {}

  auto operator()() noexcept;
  void jump() noexcept;
  void long_jump() noexcept;
  static constexpr auto min() noexcept { return uint_type{}; }
  static constexpr auto max() noexcept { return ~uint_type{}; }

  simd_type s0;
  simd_type s1;
};