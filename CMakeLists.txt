cmake_minimum_required(VERSION 3.12)
project(MyApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenCV REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(
    ${OpenCV_INCLUDE_DIRS}
    include
    thirdparty/cvui
)

add_executable(app 
    src/app.cpp
    src/gui/cv_gui.cpp
)
target_link_libraries(app 
    ${OpenCV_LIBRARIES}
)
