cmake_minimum_required(VERSION 3.5)
project(snow_net)

set(NET_SOURCE_FILES
        acceptor.cpp
        poller.cpp
        connection.cpp
        endpoint.cpp
        socket.cpp
        tcp_socket.cpp
        udp_socket.cpp
        raw_socket.cpp)
add_library(snow_net ${NET_SOURCE_FILES})