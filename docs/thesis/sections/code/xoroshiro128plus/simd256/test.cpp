TEST_CASE("simd256::xrsr128p Jump Vectorization") {
  constexpr auto simd_size = pxart::simd256::xrsr128p::simd_size;

  pxart::simd256::xrsr128p rng1{std::random_device{}};
  pxart::xrsr128p rng2[simd_size];

  for (int i = 0; i < simd_size; ++i) {
    rng2[i].s0 = reinterpret_cast<const uint64_t*>(&rng1.s0)[i];
    rng2[i].s1 = reinterpret_cast<const uint64_t*>(&rng1.s1)[i];
  }

  rng1.jump();
  for (int it = 0; it < simd_size; ++it) rng2[it].jump();

  for (int i = 0; i < simd_size; ++i) {
    const auto tmp0 = reinterpret_cast<const uint64_t*>(&rng1.s0)[i];
    const auto tmp1 = reinterpret_cast<const uint64_t*>(&rng1.s1)[i];
    CHECK(rng2[i].s0 == tmp0);
    CHECK(rng2[i].s1 == tmp1);
  }
}