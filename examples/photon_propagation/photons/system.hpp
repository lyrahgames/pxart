#pragma once
#include <immintrin.h>

#include <cmath>
#include <pxart/pxart.hpp>
#include <random>
#include <vector>

namespace photons {

struct system {
  using real_type = float;

  system() = default;
  explicit system(int n) : pos_x(n), pos_y(n), v_x(n), v_y(n), weights(n, 1) {}

  auto size() const noexcept { return pos_x.size(); }

  std::vector<real_type> pos_x{};
  std::vector<real_type> pos_y{};
  std::vector<real_type> v_x{};
  std::vector<real_type> v_y{};
  std::vector<real_type> weights{};
};

template <typename RNG>
inline void generate_random(system& sys, RNG& rng) noexcept {
  constexpr float two_pi = 6.283185307;
  std::uniform_real_distribution<system::real_type> dist{-1, 1};
  for (int i = 0; i < sys.size(); ++i) {
    const auto radius = 0.2 * std::sqrt(dist(rng));
    const auto angle = two_pi * dist(rng);
    sys.pos_x[i] = 5 * radius * std::cos(angle);
    sys.pos_y[i] = 5 * radius * std::sin(angle);
    float cos_angle = cos(-two_pi / 8);
    float sin_angle = sin(-two_pi / 8);
    const auto new_x = cos_angle * sys.pos_x[i] - sin_angle * sys.pos_y[i];
    const auto new_y = sin_angle * sys.pos_x[i] + cos_angle * sys.pos_y[i];
    sys.pos_x[i] = new_x - 10.0f;
    sys.pos_y[i] = new_y + 10.0f;
    // sys.pos_x[i] = 0 + 0.1 * dist(rng);
    // sys.pos_y[i] = 2 + 0.1 * dist(rng);
    // sys.pos_x[i] = 0.5 * dist(rng);
    // sys.pos_y[i] = 0 * dist(rng);
    // sys.v_x[i] = dist(rng);
    // sys.v_y[i] = dist(rng);
    sys.v_x[i] = 1;
    sys.v_y[i] = -1;
    const auto v_inv_norm =
        1.0f / std::sqrt(sys.v_x[i] * sys.v_x[i] + sys.v_y[i] * sys.v_y[i]);
    sys.v_x[i] *= v_inv_norm;
    sys.v_y[i] *= v_inv_norm;
  }
}

template <typename RNG>
inline void advance(system& sys, RNG& rng) noexcept {
  constexpr float two_pi = 6.283185307;
  // constexpr float g = 0.9;
  constexpr auto square = [](auto x) { return x * x; };
  std::uniform_real_distribution<system::real_type> dist{0, 1};

  for (int i = 0; i < sys.size(); ++i) {
    const auto time = -std::log(dist(rng)) * 1e-1;
    sys.pos_x[i] += time * sys.v_x[i];
    sys.pos_y[i] += time * sys.v_y[i];
    const float absorption = 0.01;
    sys.weights[i] *= (1 - absorption);
    if (dist(rng) > absorption) continue;
    // const auto angle = two_pi * dist(rng);
    // const auto cos_angle = std::cos(angle);
    // const auto sin_angle = std::sin(angle);
    // const auto cos_angle =
    //     (1 + square(g) - square((1 - g * g) / (1 - g + 2 * g * dist(rng)))) /
    //     (2 * g);
    const auto cos_angle = 2 * dist(rng) - 1;
    const auto sin_angle =
        (2 * (dist(rng) < 0.5) - 1) * std::sqrt(1 - square(cos_angle));
    const auto new_x = cos_angle * sys.v_x[i] - sin_angle * sys.v_y[i];
    const auto new_y = sin_angle * sys.v_x[i] + cos_angle * sys.v_y[i];
    // const auto new_x =
    //     cos_angle * sys.v_x[i] - sin_angle * std::sqrt(1 -
    //     square(sys.v_x[i]));
    // const auto new_y =
    //     cos_angle * sys.v_y[i] +
    //     sin_angle * sys.v_y[i] * sys.v_x[i] / std::sqrt(1 -
    //     square(sys.v_x[i]));
    sys.v_x[i] = new_x;
    sys.v_y[i] = new_y;
  }
}

namespace experimental {

template <typename RNG>
inline void advance(system& sys, RNG& rng) noexcept {
  constexpr float two_pi = 6.283185307;
  // constexpr float g = 0.9;
  constexpr auto square = [](auto x) { return x * x; };
  std::uniform_real_distribution<system::real_type> dist{0, 1};

  constexpr auto n = [](float x, float y) { return 1 + (1 - 0.5 * x * x); };
  constexpr auto grad_n = [](float x, float y) { return -1.0 * x; };

  for (int i = 0; i < sys.size(); ++i) {
    // const auto time = -std::log(dist(rng)) * 1e-2;
    const auto time = 1e-2f;
    const auto inv_n = 1 / n(sys.pos_x[i], sys.pos_y[i]);
    const auto grad = grad_n(sys.pos_x[i], sys.pos_y[i]);
    sys.pos_x[i] += time * sys.v_x[i] * inv_n;
    sys.pos_y[i] += time * sys.v_y[i] * inv_n;

    sys.v_x[i] += 0.5 * time * grad * inv_n;
    const auto v_inv_norm =
        1.0f / std::sqrt(sys.v_x[i] * sys.v_x[i] + sys.v_y[i] * sys.v_y[i]);
    sys.v_x[i] *= v_inv_norm;
    sys.v_y[i] *= v_inv_norm;

    const float absorption = 0.0001;
    const float scattering = 0.95;
    if (dist(rng) > absorption) continue;
    if (dist(rng) > scattering) {
      sys.weights[i] = 0;
      continue;
    }
    const auto angle = two_pi * dist(rng);
    const auto cos_angle = std::cos(angle);
    const auto sin_angle = std::sin(angle);
    // const auto cos_angle =
    //     (1 + square(g) - square((1 - g * g) / (1 - g + 2 * g * dist(rng)))) /
    //     (2 * g);
    // const auto cos_angle = 2 * dist(rng) - 1;
    // const auto sin_angle =
    //     (2 * (dist(rng) < 0.5) - 1) * std::sqrt(1 - square(cos_angle));
    const auto new_x = cos_angle * sys.v_x[i] - sin_angle * sys.v_y[i];
    const auto new_y = sin_angle * sys.v_x[i] + cos_angle * sys.v_y[i];
    // const auto new_x =
    //     cos_angle * sys.v_x[i] - sin_angle * std::sqrt(1 -
    //     square(sys.v_x[i]));
    // const auto new_y =
    //     cos_angle * sys.v_y[i] +
    //     sin_angle * sys.v_y[i] * sys.v_x[i] / std::sqrt(1 -
    //     square(sys.v_x[i]));
    sys.v_x[i] = new_x;
    sys.v_y[i] = new_y;
  }
}

}  // namespace experimental

namespace phase_function {

template <typename RNG>
inline void advance(system& sys, RNG& rng) noexcept {
  constexpr float two_pi = 6.283185307;
  constexpr auto square = [](auto x) { return x * x; };
  std::uniform_real_distribution<system::real_type> dist{0, 1};

  const float absorption = 0.1;
  const auto time = 1e-3f;
  const float g = 0.5f;

  for (int i = 0; i < sys.size(); ++i) {
    sys.pos_x[i] += time * sys.v_x[i];
    sys.pos_y[i] += time * sys.v_y[i];

    // const float scattering = 1.0;
    if (pxart::uniform<float>(rng) < std::exp(-absorption * time)) continue;
    // if (pxart::uniform<float>(rng) > scattering) {
    //   sys.weights[i] = 0;
    //   continue;
    // }
    const auto cos_angle =
        (1 + square(g) -
         square((1 - g * g) / (1 - g + 2 * g * pxart::uniform<float>(rng)))) /
        (2 * g);
    const auto sin_angle = ((pxart::uniform<float>(rng) > 0.5) ? 1 : -1) *
                           std::sqrt(1 - square(cos_angle));
    const auto new_x = cos_angle * sys.v_x[i] - sin_angle * sys.v_y[i];
    const auto new_y = sin_angle * sys.v_x[i] + cos_angle * sys.v_y[i];
    sys.v_x[i] = new_x;
    sys.v_y[i] = new_y;
  }
}

}  // namespace phase_function

namespace phase_function_avx {
template <typename RNG>
inline void advance(system& sys, RNG& rng) noexcept {
  constexpr float two_pi = 6.283185307;
  constexpr auto square = [](auto x) { return x * x; };
  std::uniform_real_distribution<system::real_type> dist{0, 1};

  const auto time = 1e-3f;
  const float g = 0.8f;
  const float absorption = 1.0;
  const auto free_time = std::exp(-absorption * time);

  const auto v_time = _mm256_set1_ps(time);
  const auto v_free_time = _mm256_set1_ps(free_time);

  for (int i = 0; i < sys.size(); i += 8) {
    const auto pos_x = _mm256_loadu_ps(&sys.pos_x[i]);
    const auto pos_y = _mm256_loadu_ps(&sys.pos_y[i]);
    const auto v_x = _mm256_loadu_ps(&sys.v_x[i]);
    const auto v_y = _mm256_loadu_ps(&sys.v_y[i]);
    const auto new_pos_x = _mm256_add_ps(pos_x, _mm256_mul_ps(v_time, v_x));
    const auto new_pos_y = _mm256_add_ps(pos_y, _mm256_mul_ps(v_time, v_y));
    _mm256_storeu_ps(&sys.pos_x[i], new_pos_x);
    _mm256_storeu_ps(&sys.pos_y[i], new_pos_y);

    const auto rnd =
        _mm256_set_ps(pxart::uniform<float>(rng), pxart::uniform<float>(rng),
                      pxart::uniform<float>(rng), pxart::uniform<float>(rng),
                      pxart::uniform<float>(rng), pxart::uniform<float>(rng),
                      pxart::uniform<float>(rng), pxart::uniform<float>(rng));
    const auto mask = _mm256_cmp_ps(rnd, v_free_time, _CMP_LT_OQ);

    const auto v_g = _mm256_set1_ps(g);
    const auto v_g2 = _mm256_mul_ps(v_g, v_g);
    const auto one = _mm256_set1_ps(1);
    const auto mone = _mm256_set1_ps(-1);
    const auto two = _mm256_set1_ps(2);
    const auto v_2g = _mm256_mul_ps(two, v_g);
    auto denom = _mm256_sub_ps(one, v_g);
    denom = _mm256_add_ps(denom, _mm256_mul_ps(v_2g, rnd));
    const auto numer = _mm256_sub_ps(one, v_g2);
    const auto frac = _mm256_div_ps(numer, denom);
    auto brackets = _mm256_add_ps(one, v_g2);
    brackets = _mm256_sub_ps(brackets, _mm256_mul_ps(frac, frac));
    const auto cos_angle = _mm256_div_ps(brackets, v_2g);

    const auto rnd2 =
        _mm256_set_ps(pxart::uniform<float>(rng), pxart::uniform<float>(rng),
                      pxart::uniform<float>(rng), pxart::uniform<float>(rng),
                      pxart::uniform<float>(rng), pxart::uniform<float>(rng),
                      pxart::uniform<float>(rng), pxart::uniform<float>(rng));

    const auto abs_sin_angle =
        _mm256_sqrt_ps(_mm256_sub_ps(one, _mm256_mul_ps(cos_angle, cos_angle)));
    const auto lr_mask = _mm256_cmp_ps(rnd2, _mm256_set1_ps(0.5), _CMP_LT_OQ);
    const auto sin_angle = _mm256_blendv_ps(
        abs_sin_angle, _mm256_mul_ps(mone, abs_sin_angle), lr_mask);

    const auto new_x = _mm256_sub_ps(_mm256_mul_ps(cos_angle, v_x),
                                     _mm256_mul_ps(sin_angle, v_y));
    const auto new_y = _mm256_add_ps(_mm256_mul_ps(sin_angle, v_x),
                                     _mm256_mul_ps(cos_angle, v_y));
    _mm256_storeu_ps(&sys.v_x[i], _mm256_blendv_ps(new_x, v_x, mask));
    _mm256_storeu_ps(&sys.v_y[i], _mm256_blendv_ps(new_y, v_y, mask));
  }
}
}  // namespace phase_function_avx

namespace phase_function_avx_prng {
template <typename RNG>
inline void advance(system& sys, RNG& rng) noexcept {
  const auto time = 1e-3f;
  const float g = 0.8f;
  const float absorption = 1.0;
  const auto free_time = std::exp(-absorption * time);

  const auto v_time = _mm256_set1_ps(time);
  const auto v_free_time = _mm256_set1_ps(free_time);

  for (int i = 0; i < sys.size(); i += 8) {
    const auto pos_x = _mm256_loadu_ps(&sys.pos_x[i]);
    const auto pos_y = _mm256_loadu_ps(&sys.pos_y[i]);
    const auto v_x = _mm256_loadu_ps(&sys.v_x[i]);
    const auto v_y = _mm256_loadu_ps(&sys.v_y[i]);
    const auto new_pos_x = _mm256_add_ps(pos_x, _mm256_mul_ps(v_time, v_x));
    const auto new_pos_y = _mm256_add_ps(pos_y, _mm256_mul_ps(v_time, v_y));
    // const auto new_pos_x = _mm256_fmadd_ps(v_time, v_x, pos_x);
    // const auto new_pos_y = _mm256_fmadd_ps(v_time, v_y, pos_y);
    _mm256_storeu_ps(&sys.pos_x[i], new_pos_x);
    _mm256_storeu_ps(&sys.pos_y[i], new_pos_y);

    const auto rnd = pxart::simd256::uniform<float>(rng);
    const auto mask = _mm256_cmp_ps(rnd, v_free_time, _CMP_LT_OQ);

    const auto v_g = _mm256_set1_ps(g);
    const auto v_g2 = _mm256_mul_ps(v_g, v_g);
    const auto one = _mm256_set1_ps(1);
    const auto mone = _mm256_set1_ps(-1);
    const auto two = _mm256_set1_ps(2);
    const auto v_2g = _mm256_mul_ps(two, v_g);
    auto denom = _mm256_sub_ps(one, v_g);
    denom = _mm256_add_ps(denom, _mm256_mul_ps(v_2g, rnd));
    const auto numer = _mm256_sub_ps(one, v_g2);
    const auto frac = _mm256_div_ps(numer, denom);
    auto brackets = _mm256_add_ps(one, v_g2);
    brackets = _mm256_sub_ps(brackets, _mm256_mul_ps(frac, frac));
    const auto cos_angle = _mm256_div_ps(brackets, v_2g);

    const auto rnd2 = pxart::simd256::uniform<float>(rng);

    const auto abs_sin_angle =
        _mm256_sqrt_ps(_mm256_sub_ps(one, _mm256_mul_ps(cos_angle, cos_angle)));
    const auto lr_mask = _mm256_cmp_ps(rnd2, _mm256_set1_ps(0.5), _CMP_LT_OQ);
    const auto sin_angle = _mm256_blendv_ps(
        abs_sin_angle, _mm256_mul_ps(mone, abs_sin_angle), lr_mask);

    const auto new_x = _mm256_sub_ps(_mm256_mul_ps(cos_angle, v_x),
                                     _mm256_mul_ps(sin_angle, v_y));
    const auto new_y = _mm256_add_ps(_mm256_mul_ps(sin_angle, v_x),
                                     _mm256_mul_ps(cos_angle, v_y));
    _mm256_storeu_ps(&sys.v_x[i], _mm256_blendv_ps(new_x, v_x, mask));
    _mm256_storeu_ps(&sys.v_y[i], _mm256_blendv_ps(new_y, v_y, mask));
  }
}
}  // namespace phase_function_avx_prng

namespace optics {

template <typename RNG>
inline void advance(system& sys, RNG& rng) noexcept {
  constexpr auto time = 0.5e-1f;
  std::uniform_real_distribution<system::real_type> dist{0, 1};
  constexpr auto square = [](auto x) { return x * x; };

  const float plane_n_x = 0;
  const float plane_n_y = -1;
  const float plane_d = 0;
  const float plane_t = 1.5;
  const float plane_t2 = plane_t * plane_t;

  for (int i = 0; i < sys.size(); ++i) {
    // t for plane
    const float dot_n_p = plane_n_x * sys.pos_x[i] + plane_n_y * sys.pos_y[i];
    float dot_n_v = plane_n_x * sys.v_x[i] + plane_n_y * sys.v_y[i];
    const float t = (-plane_d - dot_n_p) / dot_n_v;
    if ((0 <= t) && (t <= time)) {
      const float reflexion = 0.5;
      if (dist(rng) < reflexion) {
        // reflexion
        float new_v_x = sys.v_x[i] - 2 * dot_n_v * plane_n_x;
        float new_v_y = sys.v_y[i] - 2 * dot_n_v * plane_n_y;
        sys.pos_x[i] += t * sys.v_x[i];
        sys.pos_y[i] += t * sys.v_y[i];
        sys.v_x[i] = new_v_x;
        sys.v_y[i] = new_v_y;
        sys.pos_x[i] += (time - t) * sys.v_x[i];
        sys.pos_y[i] += (time - t) * sys.v_y[i];
      } else {
        // refraction
        float eta = plane_t;
        if (dot_n_v > 0) {
          eta = -1 / eta;
          // dot_n_v = -dot_n_v;
        }
        const float eta2 = eta * eta;
        const float sqnorm_v =
            sys.v_x[i] * sys.v_x[i] + sys.v_y[i] * sys.v_y[i];
        const float control = sqnorm_v - eta2 * (sqnorm_v - dot_n_v * dot_n_v);
        if (control <= 0) {
          float new_v_x = sys.v_x[i] - 2 * dot_n_v * plane_n_x;
          float new_v_y = sys.v_y[i] - 2 * dot_n_v * plane_n_y;
          sys.v_x[i] = new_v_x;
          sys.v_y[i] = new_v_y;
        } else {
          const float tmp = eta * std::sqrt(control);
          const float new_v_x =
              eta2 * (sys.v_x[i] - dot_n_v * plane_n_x) - tmp * plane_n_x;
          const float new_v_y =
              eta2 * (sys.v_y[i] - dot_n_v * plane_n_y) - tmp * plane_n_y;
          sys.pos_x[i] += t * sys.v_x[i];
          sys.pos_y[i] += t * sys.v_y[i];
          sys.v_x[i] = new_v_x;
          sys.v_y[i] = new_v_y;
          sys.pos_x[i] += (time - t) * sys.v_x[i];
          sys.pos_y[i] += (time - t) * sys.v_y[i];
        }
      }
    } else {
      sys.pos_x[i] += time * sys.v_x[i];
      sys.pos_y[i] += time * sys.v_y[i];

      bool inside = plane_d + dot_n_p > 0;
      const float g = (inside) ? (-0.1) : (0.999);
      const float absorption = (inside) ? (0.01) : (0);
      const float scattering = (inside) ? (1) : (1);
      if (dist(rng) < std::exp(-absorption * time)) continue;
      if (dist(rng) > scattering) {
        sys.weights[i] = 0;
        continue;
      }
      const auto cos_angle =
          (1 + square(g) - square((1 - g * g) / (1 - g + 2 * g * dist(rng)))) /
          (2 * g);
      const auto sin_angle =
          ((dist(rng) > 0.5) ? 1 : -1) * std::sqrt(1 - square(cos_angle));
      const auto new_x = cos_angle * sys.v_x[i] - sin_angle * sys.v_y[i];
      const auto new_y = sin_angle * sys.v_x[i] + cos_angle * sys.v_y[i];
      sys.v_x[i] = new_x;
      sys.v_y[i] = new_y;
    }
  }
}

}  // namespace optics

namespace optics_cache {

template <typename RNG>
inline void advance(system& sys, RNG& rng) noexcept {
  constexpr auto time = 0.5e-1f;
  std::uniform_real_distribution<system::real_type> dist{0, 1};
  constexpr auto square = [](auto x) { return x * x; };

  const float plane_n_x = 0;
  const float plane_n_y = -1;
  const float plane_d = 0;
  const float plane_t = 1.5;
  const float plane_t2 = plane_t * plane_t;

  for (int it = 0; it < sys.size(); it += 8) {
    const auto rnd1 = pxart::simd256::uniform<float>(rng);
    const auto rnd2 = pxart::simd256::uniform<float>(rng);
    const auto rnd3 = pxart::simd256::uniform<float>(rng);
    for (int j = 0; j < 8; ++j) {
      const int i = it + j;
      // t for plane
      const float dot_n_p = plane_n_x * sys.pos_x[i] + plane_n_y * sys.pos_y[i];
      float dot_n_v = plane_n_x * sys.v_x[i] + plane_n_y * sys.v_y[i];
      const float t = (-plane_d - dot_n_p) / dot_n_v;
      if ((0 <= t) && (t <= time)) {
        const float reflexion = 0.5;
        if (reinterpret_cast<const float*>(&rnd1)[j] < reflexion) {
          // reflexion
          float new_v_x = sys.v_x[i] - 2 * dot_n_v * plane_n_x;
          float new_v_y = sys.v_y[i] - 2 * dot_n_v * plane_n_y;
          sys.pos_x[i] += t * sys.v_x[i];
          sys.pos_y[i] += t * sys.v_y[i];
          sys.v_x[i] = new_v_x;
          sys.v_y[i] = new_v_y;
          sys.pos_x[i] += (time - t) * sys.v_x[i];
          sys.pos_y[i] += (time - t) * sys.v_y[i];
        } else {
          // refraction
          float eta = plane_t;
          if (dot_n_v > 0) {
            eta = -1 / eta;
            // dot_n_v = -dot_n_v;
          }
          const float eta2 = eta * eta;
          const float sqnorm_v =
              sys.v_x[i] * sys.v_x[i] + sys.v_y[i] * sys.v_y[i];
          const float control =
              sqnorm_v - eta2 * (sqnorm_v - dot_n_v * dot_n_v);
          if (control <= 0) {
            float new_v_x = sys.v_x[i] - 2 * dot_n_v * plane_n_x;
            float new_v_y = sys.v_y[i] - 2 * dot_n_v * plane_n_y;
            sys.v_x[i] = new_v_x;
            sys.v_y[i] = new_v_y;
          } else {
            const float tmp = eta * std::sqrt(control);
            const float new_v_x =
                eta2 * (sys.v_x[i] - dot_n_v * plane_n_x) - tmp * plane_n_x;
            const float new_v_y =
                eta2 * (sys.v_y[i] - dot_n_v * plane_n_y) - tmp * plane_n_y;
            sys.pos_x[i] += t * sys.v_x[i];
            sys.pos_y[i] += t * sys.v_y[i];
            sys.v_x[i] = new_v_x;
            sys.v_y[i] = new_v_y;
            sys.pos_x[i] += (time - t) * sys.v_x[i];
            sys.pos_y[i] += (time - t) * sys.v_y[i];
          }
        }
      } else {
        sys.pos_x[i] += time * sys.v_x[i];
        sys.pos_y[i] += time * sys.v_y[i];

        bool inside = plane_d + dot_n_p > 0;
        const float g = (inside) ? (-0.1) : (0.999);
        const float absorption = (inside) ? (0.01) : (0);
        const float scattering = (inside) ? (1) : (1);
        if (reinterpret_cast<const float*>(&rnd1)[j] <
            std::exp(-absorption * time))
          continue;
        const auto cos_angle =
            (1 + square(g) -
             square(
                 (1 - g * g) /
                 (1 - g + 2 * g * reinterpret_cast<const float*>(&rnd2)[j]))) /
            (2 * g);
        const auto sin_angle =
            ((reinterpret_cast<const float*>(&rnd3)[j] > 0.5) ? 1 : -1) *
            std::sqrt(1 - square(cos_angle));
        const auto new_x = cos_angle * sys.v_x[i] - sin_angle * sys.v_y[i];
        const auto new_y = sin_angle * sys.v_x[i] + cos_angle * sys.v_y[i];
        sys.v_x[i] = new_x;
        sys.v_y[i] = new_y;
      }
    }
  }
}

}  // namespace optics_cache

}  // namespace photons