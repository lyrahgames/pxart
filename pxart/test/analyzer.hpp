#pragma once

namespace pxart::test {

template <typename RNG>
class analyzer {
 public:
  using rng_type = RNG;

  explicit analyzer(const RNG& rng, int samples) : rng_{rng} {}

 private:
  rng_type rng_;
};

}  // namespace pxart::test