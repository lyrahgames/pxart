constexpr auto mt19937::operator()() noexcept -> result_type {
  // Precompute 624 transitions when all state elements have been read.
  if (state_index >= state_size) {
    // Transition for Single Element
    const auto transition = [this](int k, int k1, int k2) constexpr {
      const auto x = (state[k] & upper_mask) | (state[k1] & lower_mask);
      state[k] = state[k2] ^ (x >> 1) ^ ((x & 0x01) ? xor_mask : 0);
    };

    // Transition Loop
    // 1. Part
    for (int k = 0; k < state_size - shift_size; ++k)
      transition(k, k + 1, k + shift_size);
    // 2. Part
    for (int k = state_size - shift_size; k < state_size - 1; ++k)
      transition(k, k + 1, k + shift_size - state_size);
    // 3. Part
    transition(state_size - 1, 0, shift_size - 1);

    state_index = 0;
  }

  // Generator Function
  auto y = state[state_index++];
  y ^= (y >> tempering_u_shift);
  y ^= (y << tempering_s_shift) & tempering_b_mask;
  y ^= (y << tempering_t_shift) & tempering_c_mask;
  y ^= (y >> tempering_l_shift);
  return y;
}