template <typename result_type, typename RNG>
benchmark& run(const char* name, RNG&& rng) noexcept {
  params.setParam("npc", to_string(sizeof(result_type) / sizeof(uint32_t)));
  params.setParam("name", name);

  // Initialize cache to store temporary data.
  constexpr auto cache_count = cache_size / sizeof(result_type);
  array<result_type, cache_count> cache;
  // Warm up the cache.
  for (int i = 0; i < 100; ++i)
    for (auto& x : cache) x = rng();
  // Perform actual benchmark in new scope.
  {
    PerfEventBlock e(n * cache_count, params, header);
    for (int i = 0; i < n; ++i)
      for (auto& x : cache) x = rng();
  }
  // At end of scope, destroy PerfEventBlock and output measurements.

  header = false;
  return *this;
}
