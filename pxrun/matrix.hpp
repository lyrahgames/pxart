#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace pxrun {

class matrix {
 public:
  explicit matrix(const std::vector<std::string>& args);
  matrix() : matrix{{}} {}

  template <typename RNG>
  void run(RNG&& rng) const {
    for (int i = 0; i < rows; ++i) {
      std::cout << rng();
      for (int j = 1; j < cols; ++j) std::cout << "\t" << rng();
      std::cout << "\n";
    }
  }

 private:
  int rows = 10;
  int cols = 1;
};

}  // namespace pxrun