#include "./histogram.h"

#include <cmath>
#include <vector>


namespace iv_descriptor {

Histogram::Histogram(const std::vector<float> &values)
    : num_points_(values.size()) {
  // Compute bin size and number of bins.
  const float bin_size = ScottsRuleBinSize(values);
  if (bin_size <= 0) {
    return;
  }
  int min_val = values[0];
  int max_val = values[0];
  for (const float value : values) {
    if (value < min_val) {
      min_val = value;
    }
    if (value > max_val) {
      max_val = value;
    }
  }
  const int num_bins =
      static_cast<int>(ceil((max_val - min_val) / bin_size)) + 1;
  // Put each point into the appropriate bin.
  bins_.resize(num_bins);
  for (int index = 0; index < values.size(); ++index) {
    const int bin_id = static_cast<int>((values[index] - min_val) / bin_size);
    bins_[bin_id].push_back(index);
  }
}

// Static method.
// Uses double instead of float for intermediate computations to ensure
// precision is not lost if the given data set is very large.
float Histogram::ScottsRuleBinSize(const std::vector<float> &values) {
  if (values.size() == 0) {
    return 0;
  }
  // Compute the standard deviation of the data.
  double mean = 0;
  for (const float value : values) {
    mean += value;
  }
  mean = mean / values.size();
  double squared_diffs = 0;
  for (const float value : values) {
    const double diff = (value - mean);
    squared_diffs += diff * diff;
  }
  const double standard_deviation = sqrt((1.0 / values.size()) * squared_diffs);
  // Compute bin size using Scott's rule.
  const double bin_size = (3.49 * standard_deviation) / cbrt(values.size());
  return static_cast<float>(bin_size);
}

std::vector<int> Histogram::GetRareValues(const float fraction) const {
  std::vector<int> indices;
  const int min_bin_size = static_cast<int>(ceil(fraction * num_points_));
  // TODO(richard): if bins_ was sorted by bin size, this would be a faster
  // operation.
  for (const std::vector<int> &bin : bins_) {
    if (bin.size() <= min_bin_size) {
      for (const int point_index : bin) {
        indices.push_back(point_index);
      }
    }
  }
  return indices;
}

}  // namespace iv_descriptor
