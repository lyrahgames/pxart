TEST_CASE("simd256::mt19937 Random Initialization with Default Seeder") {
  using result_type = pxart::simd256::mt19937::result_type;
  constexpr auto simd_size = pxart::simd256::mt19937::simd_size;

  const auto seed = std::random_device{}();
  pxart::mt19937 rng{pxart::mt19937::default_seeder{seed}};
  pxart::simd256::mt19937 vrng{pxart::mt19937::default_seeder{seed}};

  const int n = 10000000;
  for (int i = 0; i < n; i += simd_size) {
    const auto v = vrng();
    for (int j = 0; j < simd_size; ++j) {
      const auto srnd = rng();
      const auto vrnd = reinterpret_cast<const result_type*>(&v)[j];
      REQUIRE(srnd == vrnd);
    }
  }
}