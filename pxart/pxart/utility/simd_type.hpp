#pragma once

#ifdef __AVX2__
#include <immintrin.h>
#endif

#ifdef __SSE4_1__
#include <emmintrin.h>
#endif

namespace pxart {

template <typename T, typename U = void>
struct simd256_type {};

#ifdef __AVX2__

template <>
struct simd256_type<float> {
  using type = __m256;
};

template <>
struct simd256_type<double> {
  using type = __m256d;
};

template <typename T>
struct simd256_type<T, std::enable_if_t<std::is_integral_v<T> > > {
  using type = __m256i;
};

#endif  // __AVX2__

template <typename T>
using simd256_t = typename simd256_type<T>::type;

}  // namespace pxart