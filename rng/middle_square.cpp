#include <cstdint>
#include <random>
#include <vector>

#include <librng/test/histogram.hpp>

using namespace std;

struct middle_square_engine {
  using result_type = uint32_t;

  result_type operator()() noexcept {
    state *= state;
    return state = (state >> 16) & 0xffffffff;
  }
  static constexpr result_type min() noexcept { return 0; }
  static constexpr result_type max() noexcept { return 0xffffffff; }

  uint64_t state = 0xfedcb2ed;
};

int main() {
  middle_square_engine ms{};
  random_device rd{};

  // vector<decltype(ms)::result_type> data{};
  // for (long i = 0; i < 10000; ++i) data.push_back(ms());
  // rng::test::sample_analyzer info{data};

  rng::test::histogram info{rd, 100000, 15};
  cout << info << '\n';
}