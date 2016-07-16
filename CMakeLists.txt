cmake_minimum_required(VERSION 3.5)
project(snow)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -o0 -std=c++11")
include_directories(../yaml-cpp/include)

set(SOURCE_FILES
        acceptor.cpp
        scheduler.cpp
        server.cpp
        logger.cpp
        event.cpp
        poller.cpp
        timer_fd.cpp
        timer.cpp
        timer_queue.cpp
        awakener.cpp
        connection.cpp
        buffer.cpp
        config.cpp)
add_library(snow_libev ${SOURCE_FILES})