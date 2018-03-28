cmake_minimum_required(VERSION 3.9)
project(AlwaysBusy)

set(CMAKE_CXX_STANDARD 11)

include_directories(lib)

set(HEADERS lib/Semaphore.h lib/tools.h)
set(SOURCES main.cpp)

add_executable(AlwaysBusy ${HEADERS} ${SOURCES})

target_link_libraries(AlwaysBusy pthread)