//
// Created by weitao on 7/19/16.
//

#pragma once

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>

namespace snow
{
    namespace utils
    {
        int get_local_addr(const std::string& interface, sockaddr* addr);
    }
}
