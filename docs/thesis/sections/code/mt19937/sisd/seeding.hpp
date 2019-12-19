struct mt19937::default_seeder {
  constexpr explicit default_seeder(uint_type s = default_seed) : x{s & mask} {}
  constexpr uint_type operator()() noexcept;
  uint_type x;
  uint_type c{0};
};

// Advancing Routine of the Default Seeder
constexpr auto mt19937::default_seeder::operator()() noexcept -> uint_type {
  const auto result = x;
  x = (x ^ (x >> (word_size - 2)));
  x = (init_multiplier * x + (++c)) & mask;
  return result;
}

// Seeding Constructor of the MT19937
template <typename RNG>
constexpr mt19937::mt19937(RNG&& rng) {
  pxart::generate(std::forward<RNG>(rng), state, state + state_size);
}