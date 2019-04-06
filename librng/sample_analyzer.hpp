#pragma once

#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include <fmt/format.h>

namespace rng {

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

  auto size() const noexcept { return counts_.size(); }
  auto max() const noexcept { return max_; }
  auto min() const noexcept { return min_; }
  auto mean() const noexcept { return mean_; }
  auto variance() const noexcept { return variance_; }
  auto standard_deviation() const noexcept { return std_dev_; }
  auto max_count() const noexcept { return max_count_; }

  const auto& counts() const noexcept { return counts_; }
  const auto& bins() const noexcept { return bins_; }
  const auto& relative_counts() const noexcept { return relative_counts_; }
  const auto& operator[](size_type index) const noexcept {
    return counts_[index];
  }
  const auto& bin(size_type index) const noexcept { return bins_[index]; }

 private:
  sample_type max_ = std::numeric_limits<sample_type>::min();
  sample_type min_ = std::numeric_limits<sample_type>::max();
  real_type mean_{};
  real_type variance_{};
  real_type std_dev_{};
  size_type max_count_{};
  std::vector<size_type> counts_{};
  std::vector<sample_type> bins_{};
  std::vector<real_type> relative_counts_{};
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
  const auto size = std::distance(first, last);

  // mean count
  for (auto it = first; it != last; ++it) {
    max_ = std::max(max_, *it);
    min_ = std::min(min_, *it);
    mean_ += *it;
  }
  mean_ /= size;

  // compute variance and standard deviation
  for (auto it = first; it != last; ++it) {
    const auto tmp = *it - mean_;
    variance_ += tmp * tmp;
  }
  variance_ /= (size - 1);
  std_dev_ = sqrt(variance_);

  const auto width = max_ - min_;
  auto bin_count = static_cast<size_type>(20);
  // bin_count = (bin_count <= width) ? (bin_count) : (width);

  // compute histogram data
  counts_.resize(bin_count, 0);
  for (auto it = first; it != last; ++it) {
    const auto index =
        static_cast<size_type>(std::floor(static_cast<real_type>(*it - min_) /
                                          (max_ + 1 - min_) * (bin_count - 1)));
    ++counts_[index];
  }

  bins_.resize(counts_.size() + 1);
  for (size_type i = 0; i < bins_.size(); ++i) {
    const auto min_sample = static_cast<sample_type>(std::ceil(
        static_cast<real_type>(i) / (bin_count - 1) * (max_ + 1 - min_) +
        min_));
    bins_[i] = min_sample;
  }

  max_count_ = counts_[0];
  for (size_type i = 1; i < counts_.size(); ++i) {
    max_count_ = std::max(max_count_, counts_[i]);
  }

  relative_counts_.resize(counts_.size());
  for (size_type i = 0; i < relative_counts_.size(); ++i)
    relative_counts_[i] = static_cast<real_type>(counts_[i]) / max_count_;
}

template <typename Integer>
std::ostream& operator<<(std::ostream& os, const sample_analyzer<Integer>& h) {
  using fmt::print;
  using std::string;
  using namespace std::string_literals;

  const auto size_of_number = [](auto x) {
    return static_cast<int>(std::ceil(std::log(x) / std::log(10)));
  };

  const auto get_label = [](std::string&& name, double max) {
    auto label_size =
        (max <= 1)
            ? (1)
            : (static_cast<int>(std::floor(std::log(max) / std::log(10))) + 1);
    label_size = (label_size < name.size()) ? (name.size()) : (label_size);
    return std::pair{std::move(name), label_size};
  };

  const auto index_label = get_label("index"s, h.size() - 1);
  const auto bin_label = get_label("bin start"s, h.max());
  const auto count_label = get_label("count"s, h.max_count());
  // const auto relative_label = get_label("relative"s, 8);
  // const auto histogram_label = get_label("histogram"s, 50);

  const auto histogram_size = 50;
  const auto table_width = 1 + index_label.second + 3 + bin_label.second + 3 +
                           count_label.second + 3 + 8 + 3 + histogram_size + 1;

  // const auto bin_label = "min sample"s;
  // auto bin_label_size = size_of_number(h.max());
  // bin_label_size = (bin_label_size < )

  print("{:->{}}\n", "", table_width);
  print(" {:^{}} | {:^{}} | {:^{}} | {:^8} | {:^{}}\n", index_label.first,
        index_label.second, bin_label.first, bin_label.second,
        count_label.first, count_label.second, "relative", "histogram",
        histogram_size);
  print("{:->{}}\n", "", table_width);
  for (auto i = 0; i < h.size(); ++i)
    print(
        " {:>{}} | {:>{}} | {:>{}} | {:>8.6f} | {:*<{}}\n", i,
        index_label.second, h.bin(i), bin_label.second, h[i],
        count_label.second, h.relative_counts()[i], "",
        static_cast<int>(std::round(histogram_size * h.relative_counts()[i])));
  print(" {:>{}} | {:>{}}\n", h.size(), index_label.second, h.bin(h.size()),
        bin_label.second);
  print("{:->{}}\n", "", table_width);

  os << "\n"
        "[min,max] = ["
     << h.min() << "," << h.max()
     << "]\n"
        "mean = "
     << h.mean()
     << "\n"
        "variance = "
     << h.variance()
     << "\n"
        "standard_deviation = "
     << h.standard_deviation() << "\n";
  return os;
}

}  // namespace rng