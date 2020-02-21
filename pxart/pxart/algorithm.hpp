#pragma once
#include <algorithm>
#include <functional>
#include <pxart/utilities/is_valid.hpp>

namespace pxart {

constexpr auto has_generate =
    is_valid([](auto&& x, auto&& y, auto&& z) -> decltype(x.generate(y, z)) {});

template <typename RNG, typename ForwardIt>
constexpr auto generate(RNG&& rng, ForwardIt first, ForwardIt last)
    -> std::enable_if_t<!decltype(has_generate(rng, first, last)){}()> {
  std::generate(first, last, std::ref(rng));
}

template <typename RNG, typename ForwardIt>
constexpr auto generate(RNG&& rng, ForwardIt first, ForwardIt last)
    -> std::enable_if_t<decltype(has_generate(rng, first, last))::value> {
  std::forward<RNG>(rng).generate(first, last);
}

}  // namespace pxart