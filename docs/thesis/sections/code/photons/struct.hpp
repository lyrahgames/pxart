struct photons {
  using real_type = float;
  std::vector<real_type> p_x{};  // Positions x-coordinate
  std::vector<real_type> p_y{};  // Positions y-coordinate
  std::vector<real_type> v_x{};  // Velocities x-direction
  std::vector<real_type> v_y{};  // Velocities y-direction
};