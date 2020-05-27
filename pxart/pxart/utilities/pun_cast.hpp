#pragma once
#include <cstring>

namespace pxart {

// This cast allows to obey strict aliasing rules while reinterpreting between
// floating-point numbers and integers.
template <typename T, typename U>
inline T pun_cast(U x) {
  static_assert(sizeof(T) == sizeof(U));
  T t{};
  std::memcpy(&t, &x, sizeof(x));
  return t;
}

}  // namespace pxart