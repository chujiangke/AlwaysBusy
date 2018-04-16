cmake_minimum_required(VERSION 3.9)
project(AlwaysBusy)

set(CMAKE_CXX_STANDARD 11)

include_directories(lib)

add_subdirectory(SingleThread)
add_subdirectory(MultiThread)
