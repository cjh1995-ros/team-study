#define CVUI_IMPLEMENTATION
#include "gui/cv_gui.h"

int main() {
  gui::OpticalFlowDebuggingApp app("path/to/video");
  app.Run();
  return 0;
}
