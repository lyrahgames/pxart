constexpr void xoroshiro128plus::jump() noexcept {
  constexpr uint_type mask[] = {0xdf900294d8f554a5ul, 0x170865df4b3201fcul};
  uint_type result0 = 0;
  uint_type result1 = 0;
  for (int i = 0; i < 2; i++) {
    for (size_t b = 0; b < word_size; b++) {
      if (mask[i] & (1ul << b)) {
        result0 ^= s0;
        result1 ^= s1;
      }
      result0 ^= s0 & tmp;
      result1 ^= s1 & tmp;
      operator()();
    }
  }
  s0 = result0;
  s1 = result1;
}