#pragma once

#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <vector>

#include <fmt/format.h>

namespace rng::test {

template <typename Integer>
class sample_analyzer {
 public:
  using sample_type = Integer;
  using real_type = double;
  using size_type = int;

  template <typename Iterator>
  sample_analyzer(Iterator first, Iterator last);
  template <typename Container>
  explicit sample_analyzer(const Container& data)
      : sample_analyzer(data.begin(), data.end()) {}

  auto sample_size() const noexcept { return sample_size_; }
  auto size() const noexcept { return counts_.size(); }
  auto max() const noexcept { return max_; }
  auto min() const noexcept { return min_; }
  auto mean() const noexcept { return mean_; }
  auto variance() const noexcept { return variance_; }
  auto standard_deviation() const noexcept { return std_dev_; }
  auto max_count() const noexcept { return max_count_; }

  const auto& counts() const noexcept { return counts_; }
  const auto& bins() const noexcept { return bins_; }
  const auto& operator[](size_type index) const noexcept {
    return counts_[index];
  }
  const auto& bin(size_type index) const noexcept { return bins_[index]; }

 private:
  size_type sample_size_{};
  sample_type max_ = std::numeric_limits<sample_type>::min();
  sample_type min_ = std::numeric_limits<sample_type>::max();
  real_type mean_{};
  real_type variance_{};
  real_type std_dev_{};
  real_type max_count_{};
  std::vector<real_type> counts_{};
  std::vector<real_type> bins_{};
};

// deduction guides
template <class Iterator>
sample_analyzer(Iterator, Iterator)
    ->sample_analyzer<typename std::iterator_traits<Iterator>::value_type>;
template <class Container>
sample_analyzer(const Container&)
    ->sample_analyzer<typename Container::value_type>;

// helper function declarations
template <typename Integer>
std::ostream& operator<<(std::ostream&, const sample_analyzer<Integer>&);

// implementations
template <typename Integer>
template <typename Iterator>
sample_analyzer<Integer>::sample_analyzer(Iterator first, Iterator last) {
  using namespace std;

  sample_size_ = distance(first, last);
  const auto [min_it, max_it] = minmax_element(first, last);
  min_ = *min_it;
  max_ = *max_it;
  mean_ = accumulate(first, last, real_type{}) / sample_size_;
  variance_ =
      inner_product(
          first, last, first, real_type{}, [](auto x, auto y) { return x + y; },
          [mean = mean_](auto x, auto y) { return (x - mean) * (y - mean); }) /
      (sample_size_ - 1);
  std_dev_ = sqrt(variance_);

  auto width = max_ - min_;
  auto bin_count = static_cast<size_type>(20);
  if constexpr (is_integral_v<sample_type>) {
    ++width;
    bin_count = (bin_count <= width) ? (bin_count) : (width);
  } else {
    width = nextafter(width, width + 1);
  }

  // compute histogram data
  counts_.resize(bin_count, 0);
  bins_.resize(counts_.size() + 1);
  for (size_type i = 0; i < bins_.size(); ++i)
    bins_[i] = static_cast<real_type>(i) / bin_count * width + min_;

  for (auto it = first; it != last; ++it) {
    const auto index = static_cast<size_type>(
        (static_cast<real_type>(*it - min_) / width * bin_count));
    if constexpr (is_integral_v<sample_type>) {
      counts_[index] +=
          clamp(bins_[index + 1] - *it, real_type{0}, real_type{1});
      counts_[index + 1] +=
          clamp(1.0 + *it - bins_[index + 1], real_type{0}, real_type{1});
    } else {
      ++counts_[index];
    }
  }

  max_count_ = *max_element(counts_.begin(), counts_.end());
}

template <typename Integer>
std::ostream& operator<<(std::ostream& os, const sample_analyzer<Integer>& h) {
  using fmt::print;
  using std::string;

  constexpr auto histogram_size = 50;
  constexpr auto table_width = 1 + 5 + 3 + 10 + 3 + 10 + 3 + histogram_size + 1;

  const auto line = string(table_width, '-') + '\n';

  print(line);
  print(" {:^5} | {:^10} | {:^10} | {:^{}}\n", "index", "bin start", "count",
        "histogram", histogram_size);
  print(line);
  for (auto i = 0; i < h.size(); ++i)
    print(" {:>5} | {:>10.6} | {:>10.6} | {:*<{}}\n", i, h.bin(i), h[i], "",
          static_cast<int>(
              std::round(histogram_size * h.counts()[i] / h.max_count())));
  print(" {:>5} | {:>10.6} |\n", h.size(), h.bin(h.size()));
  print(line);
  print(
      "[min,max] = [{},{}]\n"
      "mean = {}\n"
      "stddev = {}\n",
      h.min(), h.max(), h.mean(), h.standard_deviation());
  print(line);
  return os;
}

}  // namespace rng::test