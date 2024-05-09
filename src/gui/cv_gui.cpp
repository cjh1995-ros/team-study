#include "gui/cv_gui.h"

#include <algorithm>
#include <cmath>
#include <filesystem>
#include <iostream>

using std::sinf;

namespace fs = std::filesystem;

namespace gui {

void OpticalFlowDebuggingApp::ReadImagesFromPath(
    const std::string& video_path) noexcept {
  all_images_.clear();
  n_total_frames_ = 0;

  std::vector<std::string> file_paths;

  for (const auto& entry : fs::directory_iterator(video_path)) {
    if (entry.is_regular_file()) {
      auto filepath = entry.path();
      if (filepath.extension() == ".jpg" || filepath.extension() == ".png") {
        cv::Mat image = cv::imread(filepath.string());
        if (!image.empty()) {
          file_paths.push_back(filepath);
        }
      }
    }
  }

  all_images_.reserve(file_paths.size());

  std::sort(file_paths.begin(), file_paths.end());
  for (const auto& file_path : file_paths) {
    cv::Mat image = cv::imread(file_path);
    if (scale_factor_ != 1.0) {
      cv::resize(image, image, cv::Size(), scale_factor_, scale_factor_);
    }
    all_images_.push_back(image);
  }

  n_total_frames_ = all_images_.size();
  std::cout << "Read " << n_total_frames_ << " Images" << std::endl;
}

void OpticalFlowDebuggingApp::InitGUI() noexcept {
  cvui::init(window_name_, 20);
  image_size_ = all_images_.at(0).size();

  // Image size must be bigger than gui window size.
  window_size_ = cv::Size(3 * image_size_.width + gui_window_size_.width,
                          image_size_.height);
}

void OpticalFlowDebuggingApp::Run() {
  // Initialize images
  prev_image_ = cv::Mat::zeros(image_size_, CV_8UC3);
  cv::Mat prev_grey;
  std::vector<cv::Point2f> prev_pts;

  curr_image_ = cv::Mat::zeros(image_size_, CV_8UC3);
  cv::Mat curr_grey;
  std::vector<cv::Point2f> curr_pts;

  next_image_ = cv::Mat::zeros(image_size_, CV_8UC3);
  cv::Mat next_grey;
  std::vector<cv::Point2f> next_pts;

  while (true) {
    // Clear the window
    cv::Mat window = cv::Mat::zeros(window_size_, CV_8UC3);
    cv::Mat gui_window_ = cv::Mat::zeros(gui_window_size_, CV_8UC3);

    // Draw gui infos
    cvui::trackbar(gui_window_, 20, 20, 100, &curr_frame_id_, (int)0,
                   (int)n_total_frames_, 0, "%.0Lf");
    if (cvui::button(gui_window_, 40, 80, next_button_name_)) {
      curr_frame_id_++;
    }
    if (cvui::button(gui_window_, 40, 120, prev_button_name_)) {
      curr_frame_id_--;
    }
    if (cvui::button(gui_window_, 40, 160, quit_button_name_)) {
      is_quit_ = true;
    }
    cvui::checkbox(gui_window_, 40, 200, continue_checkbox_name_,
                   &continue_play_);

    // If continue play, Update video
    if (continue_play_) {
      if (IsCurrOutOfRange()) {
        continue_play_ = false;
        break;
      }

      if (IsPrevOutOfRange()) {
        prev_image_ = cv::Mat::zeros(image_size_, CV_8UC3);
      } else {
        prev_image_ = GetPrevImage();
        prev_grey = cv::cvtColor(prev_image_, cv::COLOR_BGR2GRAY);
      }

      if (IsNextOutOfRange()) {
        next_image_ = cv::Mat::zeros(image_size_, CV_8UC3);
      } else {
        next_image_ = GetNextImage();
        next_grey = cv::cvtColor(next_image_, cv::COLOR_BGR2GRAY);
      }

      curr_image_ = GetCurrImage();
      curr_grey = cv::cvtColor(curr_image_, cv::COLOR_BGR2GRAY);

      curr_frame_id_++;
    }
    // When stopped playing, we can watch the optical flow.
    else {
      if (IsCurrOutOfRange()) {
        break;
      }

      if (IsPrevOutOfRange()) {
        prev_image_ = cv::Mat::zeros(image_size_, CV_8UC3);
      } else {
        prev_image_ = GetPrevImage();
        prev_grey = cv::cvtColor(prev_image_, cv::COLOR_BGR2GRAY);
      }

      if (IsNextOutOfRange()) {
        next_image_ = cv::Mat::zeros(image_size_, CV_8UC3);
      } else {
        next_image_ = GetNextImage();
        next_grey = cv::cvtColor(next_image_, cv::COLOR_BGR2GRAY);
      }

      curr_image_ = GetCurrImage();
      curr_grey = cv::cvtColor(curr_image_, cv::COLOR_BGR2GRAY);
    }

    // Do algorithm in here
    if (prev_pts.empty() || !prev_grey.empty() {
      cv::goodFeaturesToTrack(prev_grey, prev_pts, 100, 0.3, 7);
    }

    optical_flow_estimator_->Estimate(prev_grey, curr_grey, prev_pts,
                                      curr_pts);

    optical_flow_estimator_->Estimate(curr_grey, next_grey, curr_pts,
                                      next_pts);

    // End algorithm

    // Draw images
    gui_window_.copyTo(window(
        cv::Rect(0, 0, gui_window_size_.width, gui_window_size_.height)));
    prev_image_.copyTo(window(cv::Rect(gui_window_size_.width, 0,
                                       image_size_.width, image_size_.height)));
    curr_image_.copyTo(
        window(cv::Rect(gui_window_size_.width + image_size_.width, 0,
                        image_size_.width, image_size_.height)));
    next_image_.copyTo(
        window(cv::Rect(gui_window_size_.width + 2 * image_size_.width, 0,
                        image_size_.width, image_size_.height)));

    cvui::update();
    cvui::imshow(window_name_, window);
    if (cv::waitKey(20) == 27 || is_quit_) {
      break;
    }
  }
}
}  // namespace gui
