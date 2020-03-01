TEST_CASE("mt19937 Random Initialization with Default Seeder") {
  const auto seed = std::random_device{}();
  std::mt19937 std_rng{seed};
  pxart::mt19937 my_rng{pxart::mt19937::default_seeder{seed}};

  const int n = 10000000;
  for (auto i = n; i > 0; --i) REQUIRE(std_rng() == my_rng());
}