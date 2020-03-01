struct xoroshiro128plus {
  // ...

  // ...

  auto operator()() noexcept;
  void jump() noexcept;
  void long_jump() noexcept;
  static constexpr auto min() noexcept { return uint_type{}; }
  static constexpr auto max() noexcept { return ~uint_type{}; }

  simd_type s0;
  simd_type s1;
};