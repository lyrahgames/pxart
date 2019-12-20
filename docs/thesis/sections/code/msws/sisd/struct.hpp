struct msws {
  using uint_type = uint64_t;
  using result_type = uint32_t;

  // ...

  // Advancing Routine
  constexpr result_type operator()() noexcept;

  // ...

  // State
  uint_type s = 0xb5ad4eceda1ce2a9;
  uint_type x = 0;
  uint_type w = 0;
};

// Advancing Routine
constexpr auto msws::operator()() noexcept -> result_type {
  x *= x;
  x += (w += s);
  return x = ((x >> 32) | (x << 32));
}