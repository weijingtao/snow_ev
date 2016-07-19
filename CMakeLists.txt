cmake_minimum_required(VERSION 3.5)
project(snow)

#set(CMAKE_C_COMPILER clang)
#set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -o0 -std=c++14")
include_directories(../yaml-cpp/include)
#include_directories(/usr/local/include)

add_subdirectory(logger)
add_subdirectory(event)
add_subdirectory(utils)
add_subdirectory(net)
add_subdirectory(timer)

include_directories(event)
include_directories(logger)
include_directories(net)
include_directories(timer)
set(SOURCE_FILES
        scheduler.cpp
        server.cpp
        awakener.cpp
        config.cpp)
add_library(snow ${SOURCE_FILES})