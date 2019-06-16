#pragma once

#include <algorithm>
// #include <any>
#include <cstdint>
#include <optional>
#include <string>

#include <boost/type_index.hpp>

#include <pxart/bit_reversal.hpp>

// TestU01 allows only one instance of an external random number generator.
// Hence, we will use hidden global and static variables together with some
// helper functions to provide a type-safe interface to TestU01 without the need
// to explicitly link to it.
namespace testu01_utils {

void set_rng_object(const std::string& name, uint32_t(oracle)());

template <typename RNG,
          typename std::enable_if_t<
              !std::is_default_constructible_v<std::decay_t<RNG>>, int> = 0>
void set_rng(std::string name, RNG&& rng, bool reverse_bits = false) noexcept;
template <typename RNG,
          typename std::enable_if_t<
              std::is_default_constructible_v<std::decay_t<RNG>>, int> = 0>
void set_rng(std::string name, RNG&& rng, bool reverse_bits = false) noexcept;
template <typename RNG>
void set_rng(RNG&& rng, bool reverse_bits = false) noexcept;

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
inline void set_rng(std::string name, RNG&& rng, bool reverse_bits) noexcept {
  // Remove colons because TestU01 cannot print them.
  std::replace(name.begin(), name.end(), ':', '_');
  // Use emplace-construction to allow for lambda expressions.
  detail::engine<RNG>.emplace(std::forward<RNG>(rng));
  if (reverse_bits)
    set_rng_object("[reversed] " + name, []() -> uint32_t {
      return pxart::bit_reversal<32>((*detail::engine<RNG>)());
    });
  else
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
inline void set_rng(std::string name, RNG&& rng, bool reverse_bits) noexcept {
  // Remove colons because TestU01 cannot print them.
  std::replace(name.begin(), name.end(), ':', '_');
  // Set global static random number engine by forwarding.
  detail::engine<RNG> = std::forward<RNG>(rng);
  if (reverse_bits)
    set_rng_object("[reversed] " + name, []() -> uint32_t {
      return pxart::bit_reversal<32>(detail::engine<RNG>());
    });
  else
    set_rng_object(name, []() -> uint32_t { return detail::engine<RNG>(); });
}

template <typename RNG>
inline void set_rng(RNG&& rng, bool reverse_bits) noexcept {
  auto name = boost::typeindex::type_id<RNG>().pretty_name();
  set_rng(name, std::forward<RNG>(rng), reverse_bits);
}

}  // namespace testu01_utils