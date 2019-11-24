struct xoroshiro128plus {
  using uint_type = uint64_t;
  using result_type = uint_type;
  static constexpr size_t word_size = 64;
  static constexpr size_t rotation_a = 24;
  static constexpr size_t shift_b = 16;
  static constexpr size_t rotation_c = 37;

  static constexpr uint_type rotate_left(uint_type x, size_t k) noexcept {
    return (x << k) | (x >> (64 - k));
  }

  xoroshiro128plus() = default;
  xoroshiro128plus(const xoroshiro128plus&) = default;
  xoroshiro128plus& operator=(const xoroshiro128plus&) = default;
  xoroshiro128plus(xoroshiro128plus&&) = default;
  xoroshiro128plus& operator=(xoroshiro128plus&&) = default;

  xoroshiro128plus(uint_type x, uint_type y) : s0{x}, s1{y} {}
  template <typename RNG>
  constexpr explicit xoroshiro128plus(RNG&& rng)
      : s0{(static_cast<uint_type>(rng()) << 32) |
           static_cast<uint_type>(rng())},
        s1{(static_cast<uint_type>(rng()) << 32) |
           static_cast<uint_type>(rng())} {}

  constexpr auto operator()() noexcept;
  constexpr void jump() noexcept;
  constexpr void long_jump() noexcept;
  static constexpr auto min() noexcept { return uint_type{}; }
  static constexpr auto max() noexcept { return ~uint_type{}; }

  uint_type s0{1314472907419283471ul};
  uint_type s1{7870872464127966567ul};
};