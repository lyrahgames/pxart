#pragma once

#include <random>
#include <utility>

namespace rng::test {

template <typename RNG>
auto monty_hall_dilemma(RNG& rng, int iterations = 1'000'000) {
  std::uniform_int_distribution<> distribution{0, 2};

  auto initial_wins = 0;
  auto switch_wins = 0;

  for (int i = 0; i < iterations; ++i) {
    const auto win = distribution(rng);
    const auto guess = distribution(rng);
    if (win == guess)
      ++initial_wins;
    else
      ++switch_wins;
  }

  return std::pair{initial_wins, switch_wins};
}

}  // namespace rng::test