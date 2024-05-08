#pragma once

#include <memory>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

#include "cvui.h"

namespace gui {

struct Location {
  int x;
  int y;
};

struct GUIContent {
  std::string text;
  Location location;
};

/// @brief Basic class which wraps GUI functionalities
class OpticalFlowDebuggingApp {
 public:
  OpticalFlowDebuggingApp(const std::string& video_path) {
    curr_frame_id_ = 0;

    window_name_ = "Optical Flow Debugging";
    trackbar_name_ = "Frame";
    continue_checkbox_name_ = "continue";
    quit_button_name_ = "quit";
    next_button_name_ = "next";
    prev_button_name_ = "prev";

    // tmp window size
    window_size_ = cv::Size(800, 600);
    // ReadImagesFromPath(video_path);
    InitGUI();
  }

  void Run();

  cv::Mat GetCurrImage() const noexcept {
    return all_images_.at(curr_frame_id_);
  }
  cv::Mat GetPrevImage() const noexcept {
    return all_images_.at(curr_frame_id_ - 1);
  }
  cv::Mat GetNextImage() const noexcept {
    return all_images_.at(curr_frame_id_ + 1);
  }

 private:
  /// @brief Read all images from the given path.
  /// @param video_path Path to the video file.
  /// @return n_total_frames, all_images_
  void ReadImagesFromPath(const std::string& video_path) noexcept;
  void InitGUI() noexcept;

  //   void RunOpticalFlow(const cv::Mat& prev, const cv::Mat& next,
  //                       const cv::Mat& prev_flow, cv::Mat& next_flow)
  //                       noexcept;

  /// @brief All images.
  std::vector<cv::Mat> all_images_;

  /// @brief cursor for visualizing
  int curr_frame_id_;

  /// @brief total frame number
  int n_total_frames_;

  bool continue_play_;

  /// @brief GUI infos
  std::string trackbar_name_;
  int trackbar_x_;
  int trackbar_y_;
  int trackbar_width_;

  std::string continue_checkbox_name_;
  bool continue_;

  std::string quit_button_name_;
  std::string next_button_name_;
  std::string prev_button_name_;

  std::string window_name_;
  // cv::Mat window_;
  cv::Size window_size_;
};
}  // namespace gui
