#include <iomanip>
#include <iostream>
#include <pxart/pxart.hpp>
#include <pxart/seed_seq.hpp>
#include <pxart/simd256/seed_seq.hpp>
#include <pxart/utilities/pun_cast.hpp>

int main() {
  using namespace std;

  pxart::seed_seq<uint32_t, 4> s{0, 0, 0x00010000, 0};
  // for (int i = 0; i < 10; ++i) cout << setw(10) << s() << '\n';
  for (auto x : s.data) cout << setw(15) << setbase(16) << x << '\n';
  for (int i = 0; i < 10; ++i) cout << setw(15) << setbase(10) << s() << '\n';
  std::array<uint32_t, 10> data;
  pxart::generate(s, begin(data), end(data));
  for (auto x : data) cout << setw(15) << setbase(10) << x << '\n';

  pxart::seed_seq_8x32 sseq{0};
  pxart::simd256::seed_seq_8x32 vseq{0};

  pxart::simd256::mt19937 rng{pxart::seed_seq_8x32{1, 2, 3, 4}};

  for (int i = 0; i < 10; ++i) {
    const auto v = pxart::pun_cast<std::array<uint32_t, 8>>(vseq());
    for (int j = 0; j < 8; ++j) {
      cout << setw(15) << setbase(16) << sseq() << setw(15) << v[j] << '\n';
    }
  }
}