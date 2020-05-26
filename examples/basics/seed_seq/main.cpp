#include <iomanip>
#include <iostream>
#include <pxart/pxart.hpp>
#include <pxart/seed_seq.hpp>

int main() {
  using namespace std;

  pxart::seed_seq<uint32_t, 4> s{0, 0, 0x00010000, 0};
  // for (int i = 0; i < 10; ++i) cout << setw(10) << s() << '\n';
  for (auto x : s.data) cout << setw(15) << setbase(16) << x << '\n';
  for (int i = 0; i < 10; ++i) cout << setw(15) << setbase(10) << s() << '\n';
  std::array<uint32_t, 10> data;
  pxart::generate(s, begin(data), end(data));
  for (auto x : data) cout << setw(15) << setbase(10) << x << '\n';
}