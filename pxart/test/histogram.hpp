#pragma once

#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <fmt/format.h>

namespace rng::test {

template <typename RNG>
class histogram {
 public:
  using rng_type = RNG;
  using real_type = double;
  using size_type = int;

  histogram() = default;
  explicit histogram(RNG& rng, size_type sample_size, size_type bin_count);

  auto sample_size() const noexcept { return sample_size_; }
  auto max_count() const noexcept { return max_count_; }
  const auto& counts() const noexcept { return counts_; }
  const auto& bins() const noexcept { return bins_; }

 private:
  std::vector<real_type> counts_{};
  std::vector<real_type> bins_{};
  size_type sample_size_{};
  real_type max_count_{};
};

template <typename RNG>
std::ostream& operator<<(std::ostream& os, const histogram<RNG>& h);

// implementations
template <typename RNG>
histogram<RNG>::histogram(RNG& rng, size_type sample_size, size_type bin_count)
    : sample_size_{sample_size} {
  if (sample_size <= 0 || bin_count <= 0)
    throw std::invalid_argument(
        "Creating a histogram of a random number generator requires positive "
        "sample size and bin count!");

  const real_type min = rng.min();
  const real_type max = rng.max();
  const auto width = max - min + 1;
  bin_count = (bin_count > width) ? (width) : (bin_count);

  // initialize bins
  bins_.resize(bin_count + 1);
  for (size_type i = 0; i < bins_.size(); ++i)
    bins_[i] = static_cast<real_type>(i) / bin_count * width + min;

  // compute counts
  counts_.resize(bin_count, 0);
  for (size_type i = 0; i < sample_size; ++i) {
    const auto sample = rng();
    const size_type index =
        static_cast<real_type>(sample - min) / width * bin_count;

    // We make two additions in two neighboring bins and clamp them afterwards
    // to get rid of aliasing artifacts.
    counts_[index] +=
        std::clamp(bins_[index + 1] - sample, real_type{0}, real_type{1});
    if (index + 1 < bin_count)
      counts_[index + 1] += std::clamp((sample + 1) - bins_[index + 1],
                                       real_type{0}, real_type{1});
  }

  max_count_ = *max_element(counts_.begin(), counts_.end());
}

template <typename RNG>
std::ostream& operator<<(std::ostream& os, const histogram<RNG>& h) {
  using fmt::print;
  using std::string;

  constexpr auto histogram_size = 50;
  constexpr auto table_width = 1 + 5 + 3 + 10 + 3 + 10 + 3 + histogram_size + 1;

  const auto line = string(table_width, '-') + '\n';

  print(line);
  print(" {:^5} | {:^10} | {:^10} | {:^{}}\n", "bin", "bin start", "count",
        "histogram", histogram_size);
  print(line);
  for (auto i = 0; i < h.counts().size(); ++i)
    print(" {:>5} | {:>10.0f} | {:>10.0f} | {:*<{}}\n", i, h.bins()[i],
          h.counts()[i], "",
          static_cast<int>(
              std::round(histogram_size * h.counts()[i] / h.max_count())));
  print(" {:>5} | {:>10.0f} |\n", h.counts().size(), h.bins().back());
  print(line);
  return os;
}

}  // namespace rng::test