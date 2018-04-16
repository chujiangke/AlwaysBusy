cmake_minimum_required(VERSION 3.9)
project(AlwaysBusy)

add_executable(SingleThread ../lib/Array.h main.cpp)

target_link_libraries(SingleThread pthread )
