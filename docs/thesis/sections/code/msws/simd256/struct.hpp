struct msws {
  using uint_type = uint64_t;
  using result_type = uint32_t;
  using simd_type = __m256i;
  static constexpr size_t simd_size = 8;

  static simd_type _mm256_square_epi64(simd_type x) noexcept;
  simd_type operator()() noexcept;

  template <typename RNG>
  static constexpr uint_type seed(RNG&& rng) {
    return (static_cast<uint_type>(rng()) << 32) | (rng() << 1) | 0x01;
  }
  template <typename RNG>
  explicit msws(RNG&& rng)
      : step{_mm256_set_epi64x(seed(rng), seed(rng), seed(rng), seed(rng)),
             _mm256_set_epi64x(seed(rng), seed(rng), seed(rng), seed(rng))},
        root{_mm256_setzero_si256(), _mm256_setzero_si256()},
        weyl{_mm256_setzero_si256(), _mm256_setzero_si256()} {}

  simd_type step[2];
  simd_type root[2];
  simd_type weyl[2];
};