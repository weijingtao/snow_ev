cmake_minimum_required(VERSION 3.5)
project(samples)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -o0 -std=c++11")

include_directories(..)
link_directories(..)

add_executable(echo echo.cpp)
target_link_libraries(echo event logger net timer)