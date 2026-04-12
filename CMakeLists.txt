cmake_minimum_required(VERSION 3.10)
project(CarCounter)

# Use Modern C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# ---> ADD YOUR E DRIVE PATH HERE <---
set(OpenCV_DIR "E:/Apps/opencv/build")

# Find OpenCV
find_package(OpenCV REQUIRED)

# Include directories
include_directories(${OpenCV_INCLUDE_DIRS})

# Create the executable (Notice we added tinyfiledialogs.c here)
add_executable(CarCounter main.cpp tinyfiledialogs.c)

# Link OpenCV libraries
target_link_libraries(CarCounter ${OpenCV_LIBS})