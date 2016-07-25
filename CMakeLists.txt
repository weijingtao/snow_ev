cmake_minimum_required(VERSION 3.5)
project(snow)

#set(CMAKE_C_COMPILER clang)
#set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -o0 -std=c++11")
include_directories(../yaml-cpp/include)
include_directories(/usr/local/include)
include_directories(.)


set(LOGGER_SOURCE_FILES
        logger/logger.cpp)

set(EVENT_SOURCE_FILES
        event/event.cpp
        event/poller.cpp)

set(TIMER_SOURCE_FILES
        timer/timer.cpp
        timer/timer_fd.cpp
        timer/timer_queue.cpp)

set(UTILS_SOURCE_FILES
        utils/net_helper.cpp)

set(NET_SOURCE_FILES
        net/acceptor.cpp
        net/buffer.cpp
        net/connection.cpp
        net/endpoint.cpp
        net/socket.cpp
        net/raw_socket.cpp
        net/tcp_socket.cpp
        net/udp_socket.cpp)

set(FRAME_SOURCE_FILES
        scheduler.cpp
        server.cpp
        awakener.cpp
        config.cpp)
add_library(snow ${FRAME_SOURCE_FILES} ${LOGGER_SOURCE_FILES} ${EVENT_SOURCE_FILES} ${TIMER_SOURCE_FILES} ${UTILS_SOURCE_FILES} ${NET_SOURCE_FILES})

add_subdirectory(samples)