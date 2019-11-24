constexpr auto xoroshiro128plus::operator()() noexcept {
  // The order is important. Otherwise jumps will not work properly.
  const auto result = s0 + s1;
  s1 ^= s0;
  s0 = rotate_left(s0, rotation_a) ^ s1 ^ (s1 << shift_b);
  s1 = rotate_left(s1, rotation_c);
  return result;
}

constexpr void xoroshiro128plus::jump() noexcept {
  // Magic numbers depend on rotation and shift arguments.
  constexpr uint_type mask[] = {0xdf900294d8f554a5ul, 0x170865df4b3201fcul};
  uint_type result0 = 0;
  uint_type result1 = 0;
  for (int i = 0; i < 2; i++) {
    for (size_t b = 0; b < word_size; b++) {
      // if (mask[i] & (1ul << b)) {
      //   result0 ^= s0;
      //   result1 ^= s1;
      // }
      const auto tmp = (mask[i] & (1ul << b)) ? (~uint_type{}) : (0);
      result0 ^= s0 & tmp;
      result1 ^= s1 & tmp;
      operator()();
    }
  }
  s0 = result0;
  s1 = result1;
}