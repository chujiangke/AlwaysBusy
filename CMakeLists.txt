cmake_minimum_required(VERSION 3.9)
project(AlwaysBusy)

set(CMAKE_CXX_STANDARD 11)

include_directories(lib)

file(GLOB HEADERS lib/*.h)
file(GLOB SOURCES main.cpp lib/*.cpp)

add_executable(AlwaysBusy ${HEADERS} ${SOURCES})

target_link_libraries(AlwaysBusy pthread)