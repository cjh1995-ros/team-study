#define CVUI_IMPLEMENTATION
#include "gui/cv_gui.h"

int main() {
  gui::OpticalFlowDebuggingApp app("../data/paris");
  app.Run();
  return 0;
}
