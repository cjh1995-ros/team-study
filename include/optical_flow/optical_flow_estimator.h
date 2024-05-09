#pragma once

#include <opencv2/opencv.hpp>
#include <vector>

namespace vision {

// This is basic opencv optical flow estimator
class OpticalFlowEstimator {
 public:
  OpticalFlowEstimator() = default;
  ~OpticalFlowEstimator() = default;

  std::vector<uchar> Estimate(const cv::Mat& prev, const cv::Mat& next,
                              std::vector<cv::Point2f>& prev_pts,
                              std::vector<cv::Point2f>& next_pts) noexcept;
};

}  // namespace vision