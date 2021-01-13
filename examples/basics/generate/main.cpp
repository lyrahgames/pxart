#include <iomanip>
#include <iostream>
#include <random>
#include <vector>
//
#include <pxart/algorithm.hpp>
#include <pxart/mt19937.hpp>
#include <pxart/uniform.hpp>

using namespace std;

int main() {
  pxart::mt19937 rng{random_device{}};

  for (size_t i = 0; i < 10; ++i) {
    vector<char> letters(20);
    // Call generate algorithm to generate random 20-character-wide vector.
    pxart::generate([&rng]() { return uniform<char>(rng, 'a', 'z'); },
                    begin(letters), end(letters));

    for (auto c : letters) cout << c;
    cout << '\n';
  }
}