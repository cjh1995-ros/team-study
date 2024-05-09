#include "optical_flow/optical_flow_estimator.h"

namespace vision {

std::vector<uchar> OpticalFlowEstimator::Estimate(
    const cv::Mat& prev, const cv::Mat& next,
    std::vector<cv::Point2f>& prev_pts,
    std::vector<cv::Point2f>& next_pts) noexcept {
  std::vector<uchar> status;
  std::vector<float> err;
  cv::calcOpticalFlowPyrLK(prev, next, prev_pts, next_pts, status, err);
  return status;
}

}  // namespace vision