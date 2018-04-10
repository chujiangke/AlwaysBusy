cmake_minimum_required(VERSION 3.9)
project(AlwaysBusy)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)

include_directories(lib)
include_directories(${OPENCV_INCLUDE_DIRS})

file(GLOB HEADERS lib/*.h)
file(GLOB SOURCES main.cpp lib/*.cpp)

add_executable(AlwaysBusy ${HEADERS} ${SOURCES} lib/Array.h)

target_link_libraries(AlwaysBusy pthread ${OpenCV_LIBS})