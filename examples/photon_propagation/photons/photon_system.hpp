#pragma once
#include <vector>

namespace photons::scalar {

struct photon {
  float p_x{};
  float p_y{};
  float v_x{1};
  float v_y{};
};

struct plane {
  float n_x{1};
  float n_y{};
  float d{};
};

inline float intersection(const plane& n, const photon& p) noexcept {
  const float dot_n_p = n.n_x * p.p_x + n.n_y * p.p_y;
  float dot_n_v = n.n_x * p.v_x + n.n_y * p.v_y;
  const float t = (-n.d - dot_n_p) / dot_n_v;
}

struct system {
  std::vector<photon> photons;
  std::vector<plane> planes;
  float time = 0.5e-1f;
};

template <typename RNG>
inline void advance(system& sys, RNG&& rng) noexcept {
  for (int i = 0; i < sys.size(); ++i) {
    if ((0 <= t) && (t <= time)) {
      float new_v_x = sys.v_x[i] - 2 * dot_n_v * plane_n_x;
      float new_v_y = sys.v_y[i] - 2 * dot_n_v * plane_n_y;
      sys.pos_x[i] += t * sys.v_x[i];
      sys.pos_y[i] += t * sys.v_y[i];
      sys.v_x[i] = new_v_x;
      sys.v_y[i] = new_v_y;
      sys.pos_x[i] += (time - t) * sys.v_x[i];
      sys.pos_y[i] += (time - t) * sys.v_y[i];
    }
  }
}

}  // namespace photons::scalar