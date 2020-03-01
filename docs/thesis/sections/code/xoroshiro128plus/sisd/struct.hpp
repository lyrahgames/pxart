struct xoroshiro128plus {
  using uint_type = uint64_t;
  using result_type = uint_type;
  static constexpr size_t word_size = 64;
  static constexpr size_t rotation_a = 24;
  static constexpr size_t shift_b = 16;
  static constexpr size_t rotation_c = 37;

  // ...

  constexpr auto operator()() noexcept;
  constexpr void jump() noexcept;

  // ...

  // State
  uint_type s0{1314472907419283471ul};
  uint_type s1{7870872464127966567ul};
};