cmake_minimum_required(VERSION 3.5)
project(snow_utils)
set(UTILS_SOURCE_FILES
        net_helper.cpp)
add_library(snow_utils ${UTILS_SOURCE_FILES})