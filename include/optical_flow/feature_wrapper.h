#pragma once

#include <memory>
#include <opencv2/opencv.hpp>
#include <vector>

namespace vision {

struct Feature : public std::enable_shared_from_this<Feature> {
  using Ptr = std::shared_ptr<Feature>;
  cv::Point2f pt;
  Ptr prev;
  Ptr next;
  /// @brief Indicates if the feature is being tracked
  bool is_tracked;
  /// @brief Unique id of the feature in the image
  int id_in_image;
  int id_in_features;
  /// @brief Age of the feature
  int age;
};

typedef PtrFeature std::shared_ptr<Feature>;

}  // namespace vision