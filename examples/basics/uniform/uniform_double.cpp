#include <iomanip>
#include <iostream>
#include <pxart/pxart.hpp>

int main() {
  using namespace std;
  pxart::mt19937 rng{random_device{}};
  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 4; ++j) {
      cout << fixed << setw(16) << setprecision(12)
           << pxart::uniform<double>(rng);
    }
    cout << '\n';
  }
}