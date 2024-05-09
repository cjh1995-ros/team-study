#pragma once

#include <memory>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

#include "cvui.h"
#include "optical_flow/optical_flow_estimator.h"

namespace gui {

/// @brief Basic class which wraps GUI functionalities
class OpticalFlowDebuggingApp {
 public:
  OpticalFlowDebuggingApp(const std::string& video_path) {
    window_name_ = "Optical Flow Debugging";
    trackbar_name_ = "Frame";
    continue_checkbox_name_ = "continue";
    continue_play_ = false;
    quit_button_name_ = "quit";
    is_quit_ = false;
    next_button_name_ = "next";
    prev_button_name_ = "prev";
    curr_frame_id_ = 0;
    scale_factor_ = 0.5;

    gui_window_size_ = cv::Size(150, 250);
    ReadImagesFromPath(video_path);
    InitGUI();
  }

  void Run();

  cv::Mat GetCurrImage() const noexcept {
    return all_images_.at(curr_frame_id_);
  }
  bool IsCurrOutOfRange() const noexcept {
    return curr_frame_id_ >= n_total_frames_ - 1 || curr_frame_id_ < 0;
  }
  cv::Mat GetPrevImage() const noexcept {
    return all_images_.at(curr_frame_id_ - 1);
  }
  bool IsPrevOutOfRange() const noexcept { return curr_frame_id_ - 1 < 0; }
  cv::Mat GetNextImage() const noexcept {
    return all_images_.at(curr_frame_id_ + 1);
  }
  bool IsNextOutOfRange() const noexcept {
    return curr_frame_id_ >= n_total_frames_;
  }

 private:
  /// @brief Read all images from the given path.
  /// @param video_path Path to the video file.
  /// @return n_total_frames, all_images_
  void ReadImagesFromPath(const std::string& video_path) noexcept;
  void InitGUI() noexcept;

  /// @brief All images.
  std::vector<cv::Mat> all_images_;
  cv::Size image_size_;
  double scale_factor_;
  int n_total_frames_;

  /// @brief Optical flow estimator
  std::shared_ptr<vision::OpticalFlowEstimator> optical_flow_estimator_;

  /// @brief cursor for visualizing
  int curr_frame_id_;
  cv::Mat curr_image_, prev_image_, next_image_;

  /// @brief GUI infos
  std::string trackbar_name_;
  int trackbar_x_;
  int trackbar_y_;
  int trackbar_width_;

  std::string continue_checkbox_name_;
  bool continue_play_;

  std::string quit_button_name_;
  bool is_quit_;

  std::string next_button_name_;
  std::string prev_button_name_;

  std::string window_name_;
  cv::Size gui_window_size_;

  cv::Size window_size_;
};
}  // namespace gui
