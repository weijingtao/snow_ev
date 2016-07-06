cmake_minimum_required(VERSION 3.5)
project(snow)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        acceptor.cpp
        scheduler.cpp
        server.cpp
        logger.cpp)
add_executable(snow_libev ${SOURCE_FILES})