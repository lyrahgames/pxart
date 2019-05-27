#pragma once

#include <algorithm>
// #include <any>
#include <boost/type_index.hpp>
#include <cstdint>
#include <optional>

// TestU01 allows only one instance of an external random number generator.
// Hence, we will use hidden global and static variables together with some
// helper functions to provide a type-safe interface to TestU01 without the need
// to explicitly link to it.
namespace testu01_utils {

void set_rng_object(const char* name, uint32_t(oracle)());

template <typename RNG,
          typename std::enable_if_t<
              !std::is_default_constructible_v<std::decay_t<RNG>>, int> = 0>
void set_rng(const char* name, RNG&& rng) noexcept;
template <typename RNG,
          typename std::enable_if_t<
              std::is_default_constructible_v<std::decay_t<RNG>>, int> = 0>
void set_rng(const char* name, RNG&& rng) noexcept;
template <typename RNG>
void set_rng(RNG&& rng) noexcept;

void verbose(bool = true) noexcept;
void run_battery_small_crush() noexcept;
void run_battery_crush() noexcept;
void run_battery_big_crush() noexcept;
void run_test_linear_complexity() noexcept;

// std::any is the most general solution but it introduces overhead.
// We can do better, especially for testing well-defined generators.
//
// inline std::any engine;
// template <typename RNG>
// inline void set_rng(const char* name, RNG&& rng) noexcept {
//   engine = std::forward<RNG>(rng);
//   set_rng_object(name, []() -> uint32_t {
//     return std::any_cast<std::decay_t<RNG>&>(engine)();
//   });
// }

// std::optional introduces overhead as well. But it is better than std::any.
// We only need std::optional if there is no default constructor (eg. lambda
// expressions). Therefore we specialize the variable template for default
// constructors to improve run-time performance while testing.
namespace detail {

template <typename RNG, typename = void>
std::optional<std::decay_t<RNG>> engine{};
template <typename RNG>
std::decay_t<RNG> engine<
    RNG,
    std::enable_if_t<std::is_default_constructible_v<std::decay_t<RNG>>>>{};

}  // namespace detail

// Function for types that are not default constructible.
template <typename RNG,
          typename std::enable_if_t<
              !std::is_default_constructible_v<std::decay_t<RNG>>, int> = 0>
inline void set_rng(const char* name, RNG&& rng) noexcept {
  // Use emplace-construction to allow for lambda expressions.
  detail::engine<RNG>.emplace(std::forward<RNG>(rng));
  set_rng_object(name, []() -> uint32_t {
    return (*detail::engine<RNG>)();
    // bad: takes longer than other approach
    // return detail::engine<rng_type>.value()();
  });
}

// Function for default-constructible types.
template <typename RNG,
          typename std::enable_if_t<
              std::is_default_constructible_v<std::decay_t<RNG>>, int> = 0>
inline void set_rng(const char* name, RNG&& rng) noexcept {
  detail::engine<RNG> = rng;
  set_rng_object(name, []() -> uint32_t { return detail::engine<RNG>(); });
}

template <typename RNG>
inline void set_rng(RNG&& rng) noexcept {
  auto name = boost::typeindex::type_id<RNG>().pretty_name();
  std::replace(name.begin(), name.end(), ':', '_');
  set_rng(name.c_str(), std::forward<RNG>(rng));
}

}  // namespace testu01_utils