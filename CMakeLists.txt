cmake_minimum_required(VERSION 3.9)
project(AlwaysBusy)

set(CMAKE_C_COMPILER gcc)

include_directories(lib)

set(HEADERS lib/Semaphore.h)
set(SOURCES main.c lib/Semaphore.c)

add_executable(AlwaysBusy ${HEADERS} ${SOURCES})

target_link_libraries(AlwaysBusy pthread)