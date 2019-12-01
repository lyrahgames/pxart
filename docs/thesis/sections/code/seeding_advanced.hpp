struct rng_type {
  // ...
  template <  //
      typename RNG,
      typename = std::enable_if_t<!std::is_same_v<rng_type, std::decay_t<RNG>>>>
  explicit rng_type(RNG&& rng);

  rng_type(const rng_type&) = default;
  rng_type& operator=(const rng_type&) = default;
  rng_type(rng_type&&) = default;
  rng_type& operator=(rng_type&&) = default;
  // ...
};
