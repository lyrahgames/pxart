constexpr auto xoroshiro128plus::operator()() noexcept {
  constexpr auto rotate_left = [](uint_type x, size_t k) -> uint_type {
    return (x << k) | (x >> (64 - k));
  };

  const auto result = s0 + s1;
  s1 ^= s0;
  s0 = rotate_left(s0, rotation_a) ^ s1 ^ (s1 << shift_b);
  s1 = rotate_left(s1, rotation_c);
  return result;
}