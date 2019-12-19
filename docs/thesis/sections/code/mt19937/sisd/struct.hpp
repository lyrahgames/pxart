struct mt19937 {
  // ...

  struct default_seeder;

  // Seeding Constructor
  template <typename RNG>
  constexpr explicit mt19937(RNG&& rng);

  // Advancing Routine
  constexpr result_type operator()() noexcept;

  // State
  uint_type state[state_size];
  int state_index = state_size;
};