//
// Created by weitao on 7/19/16.
//

#include "net_helper.h"

#include <errno.h>
#include <cstring>
#include "../logger/logger.h"

namespace snow
{
    namespace utils
    {
        int get_local_addr(const std::string& interface, sockaddr* addr) {
            struct ifaddrs *ifaddr = nullptr, *ifa = nullptr;
            int family, s;
            char host[NI_MAXHOST];

            if (getifaddrs(&ifaddr) == -1) {
                SNOW_LOG_FATAL << "getifaddrs failed, errno:" << errno << ", errmsg:" << ::strerror(errno);
                freeifaddrs(ifaddr);
            }

            /* Walk through linked list, maintaining head pointer so we
             *        can free list later */

            for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
                if ((nullptr != ifa->ifa_addr) && interface.compare(ifa->ifa_name)) {
                    *addr = *(ifa->ifa_addr);
                    freeifaddrs(ifaddr);
                    return 0;
                }
            }
            freeifaddrs(ifaddr);
            return -1;
        }
    }
}

