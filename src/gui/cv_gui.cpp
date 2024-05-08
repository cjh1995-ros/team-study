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

  std::sort(file_paths.begin(), file_paths.end());
  for (const auto& file_path : file_paths) {
    cv::Mat image = cv::imread(file_path);
    all_images_.push_back(image);
  }

  n_total_frames_ = all_images_.size();
  std::cout << "Read " << n_total_frames_ << " Images" << std::endl;
}

void OpticalFlowDebuggingApp::InitGUI() noexcept {
  cvui::init(window_name_, 20);
}

void OpticalFlowDebuggingApp::Run() {
  cv::Size window_size_ = cv::Size(800, 600);
  bool checked = false;
  while (true) {
    cv::Mat window_ = cv::Mat::zeros(window_size_, CV_8UC3);

    cvui::trackbar(window_, 40, 30, 220, &curr_frame_id_, (int)0, (int)100, 0,
                   "%.0Lf");
    cvui::checkbox(window_, 90, 100, continue_checkbox_name_, &checked);

    if (cvui::button(window_, 300, 40, quit_button_name_)) {
      std::cout << "Quit button clicked" << std::endl;
      break;
    }

    if (cvui::button(window_, 400, 40, next_button_name_)) {
      std::cout << "Next button clicked" << std::endl;
    }

    if (cvui::button(window_, 500, 40, prev_button_name_)) {
      std::cout << "Prev button clicked" << std::endl;
    }

    cvui::update();
    cvui::imshow(window_name_, window_);
    if (cv::waitKey(20) == 27) {
      break;
    }
  }
}
}  // namespace gui
