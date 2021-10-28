#pragma once

#include <algorithm>
#include <cmath>
#include <functional>
#include <numeric>
#include <vector>

namespace Shared {
class Statistics {
public:
  struct Quartiles {
    double LowerOuterFence;
    double LowerInnerFence;
    double Q1;
    double Median;
    double Q3;
    double UpperInnerFence;
    double UpperOuterFence;
  };

  template <class INPUT_IT>
  static Quartiles CalculateQuartiles(
      const INPUT_IT begin, const INPUT_IT end,
      std::function<double(typename std::iterator_traits<INPUT_IT>::value_type)>
          getX = [](typename std::iterator_traits<INPUT_IT>::value_type value) {
            return value;
          }) {
    const size_t dataSize{size_t(std::abs(std::distance(begin, end)))};
    std::vector<double> dataToSort;
    dataToSort.reserve(dataSize);
    for (auto it = begin; it != end; ++it) {
      dataToSort.push_back(getX(*it));
    }

    std::sort(dataToSort.begin(), dataToSort.end());

    Quartiles rslt;

    if (dataSize % 2 == 0) {
      size_t medianIndex0 = dataSize / 2 - 1;
      size_t medianIndex1 = medianIndex0 + 1;

      rslt.Median = (dataToSort[medianIndex0] + dataToSort[medianIndex1]) / 2.0;

      if (dataSize % 4 == 0) {
        size_t quartile1Index0 = dataSize / 4 - 1;
        size_t quartile1Index1 = quartile1Index0 + 1;

        size_t quartile3Index0 = 3 * (dataSize / 4) - 1;
        size_t quartile3Index1 = quartile3Index0 + 1;

        rslt.Q1 =
            (dataToSort[quartile1Index0] + dataToSort[quartile1Index1]) / 2.0;
        rslt.Q3 =
            (dataToSort[quartile3Index0] + dataToSort[quartile3Index1]) / 2.0;
      } else {
        size_t quartile1Index = size_t(double(dataSize) / 4.0 - 0.5);
        size_t quartile3Index = size_t(3.0 * double(dataSize) / 4.0 - 0.5);

        rslt.Q1 = dataToSort[quartile1Index];
        rslt.Q3 = dataToSort[quartile3Index];
      }
    } else {
      size_t medianIndex = size_t(double(dataSize) / 2.0 - 0.5);
      size_t quartile1Index = size_t(double(medianIndex) / 2.0 - 0.5);
      size_t quartile3Index = medianIndex + (quartile1Index + 1);
      rslt.Q1 = dataToSort[quartile1Index];
      rslt.Median = dataToSort[medianIndex];
      rslt.Q3 = dataToSort[quartile3Index];
    }

    double iqr = std::abs(rslt.Q1 - rslt.Q3);
    double lowerFence = iqr * 1.5;
    double outerFence = iqr * 3;
    rslt.LowerInnerFence = rslt.Q1 - lowerFence;
    rslt.LowerOuterFence = rslt.Q1 - outerFence;
    rslt.UpperInnerFence = rslt.Q3 + lowerFence;
    rslt.UpperOuterFence = rslt.Q3 + outerFence;

    return rslt;
  }

  template <class INPUT_IT>
  static double Median(
      const INPUT_IT begin, const INPUT_IT end,
      std::function<double(typename std::iterator_traits<INPUT_IT>::value_type)>
          getX = [](typename std::iterator_traits<INPUT_IT>::value_type value) {
            return value;
          }) {
    const size_t dataSize{size_t(std::abs(std::distance(begin, end)))};
    std::vector<double> dataToSort;
    dataToSort.reserve(dataSize);
    for (auto it = begin; it != end; ++it) {
      dataToSort.push_back(getX(*it));
    }

    std::sort(dataToSort.begin(), dataToSort.end());

    if (dataToSort.size() % 2 == 0) {
      size_t medianIndex0 = dataSize / 2 - 1;
      size_t medianIndex1 = medianIndex0 + 1;

      return (dataToSort[medianIndex0] + dataToSort[medianIndex1]) / 2.0;
    }

    size_t medianIndex = size_t(double(dataToSort.size()) / 2.0 - 0.5);
    return dataToSort[medianIndex];
  }

  template <class INPUT_IT>
  static double Mean(
      const INPUT_IT begin, const INPUT_IT end,
      std::function<double(typename std::iterator_traits<INPUT_IT>::value_type)>
          getX = [](typename std::iterator_traits<INPUT_IT>::value_type value) {
            return value;
          }) {
    auto accum_func =
        [&getX](
            double accumulator,
            const typename std::iterator_traits<INPUT_IT>::value_type &value) {
          return accumulator + getX(value);
        };

    return double(std::accumulate(begin, end, double(0.0), accum_func) /
                  std::distance(begin, end));
  }

  template <class T> static T LinearInterpolate(T x1, T x2, T y1, T y2, T loc) {
    return y1 + (loc - x1) * ((y2 - y1) / (x2 - x1));
  }

  template <class INPUT_IT>
  static double StdDeviation(
      const INPUT_IT begin, const INPUT_IT end, double mean,
      std::function<double(typename std::iterator_traits<INPUT_IT>::value_type)>
          getX) {
    auto accum_func =
        [&getX, &mean](
            double accumulator,
            const typename std::iterator_traits<INPUT_IT>::value_type &value) {
          return accumulator + ((getX(value) - mean) * (getX(value) - mean));
        };

    return std::sqrt(std::accumulate(begin, end, 0.0, accum_func) /
                     std::distance(begin, end));
  }

  template <class INPUT_IT>
  static double StdDeviation(
      const INPUT_IT begin, const INPUT_IT end,
      std::function<double(typename std::iterator_traits<INPUT_IT>::value_type)>
          getX = [](typename std::iterator_traits<INPUT_IT>::value_type value) {
            return value;
          }) {
    double mean = Mean(begin, end, getX);

    return StdDeviation(begin, end, mean, getX);
  }
};
} // namespace Shared