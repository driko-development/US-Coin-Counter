project (program2)
cmake_minimum_required(VERSION 3.15)

# configure OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_library(lib coinsortmachine.h coinsortmachine.cpp)

# create create individual projects
add_executable(program2 program2.cpp)
target_link_libraries(program2 ${OpenCV_LIBS} lib)