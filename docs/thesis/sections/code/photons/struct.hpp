struct system {
  // System of Photons
  std::vector<float> p_x{};  // Positions x-coordinate
  std::vector<float> p_y{};  // Positions y-coordinate
  std::vector<float> v_x{};  // Velocities x-direction
  std::vector<float> v_y{};  // Velocities y-direction

  // Medium
  float scatter{0.1f};  // Scattering Probability
  float g{0.8f};        // Asymmetry Parameter

  // Discretization
  float time_step{1e-3f};
};