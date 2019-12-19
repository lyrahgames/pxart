struct mt19937 {
  // ...
  using simd_type = __m256i;
  static constexpr size_t simd_size = sizeof(simd_type) / sizeof(result_type);

  // ...

  // Advancing Routine
  simd_type operator()() noexcept;

  // ...

  // State with Buffer
  uint_type state[state_size + simd_size] __attribute__((aligned(32)));
  int state_index = state_size;
};