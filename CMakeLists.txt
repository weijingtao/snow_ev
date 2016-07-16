cmake_minimum_required(VERSION 3.5)
project(snow)

#set(CMAKE_C_COMPILER clang)
#set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -o0 -std=c++11")
include_directories(../yaml-cpp/include)

add_subdirectory(event)
add_subdirectory(logger)
add_subdirectory(net)
set(SOURCE_FILES
        scheduler.cpp
        server.cpp
        awakener.cpp
        buffer.cpp
        config.cpp)
add_library(snow_libev ${SOURCE_FILES})