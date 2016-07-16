cmake_minimum_required(VERSION 3.5)
project(snow_timer)

include_directories(../event)
set(TIMER_SOURCE_FILES
        timer_fd.cpp
        timer.cpp
        timer_queue.cpp)
add_library(snow_timer ${TIMER_SOURCE_FILES})