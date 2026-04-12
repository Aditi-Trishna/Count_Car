cmake_minimum_required(VERSION 3.10)
project(CarCounter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# We removed the hardcoded Windows E:/ drive line!
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(CarCounter main.cpp tinyfiledialogs.c)
target_link_libraries(CarCounter ${OpenCV_LIBS})