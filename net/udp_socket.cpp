//
// Created by weitao on 7/16/16.
//

#include "udp_socket.h"

namespace snow
{
    udp_socket::udp_socket(int fd)
        : socket(fd) {

    }

    std::size_t udp_socket::read(buffer* buf) {
        return 0;
    }

    std::size_t udp_socket::write(buffer& buf) {
        return 0;
    }
}