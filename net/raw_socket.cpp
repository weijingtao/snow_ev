//
// Created by weitao on 7/16/16.
//

#include "raw_socket.h"

namespace snow
{
    raw_socket::raw_socket(int fd)
        : socket(fd) {

    }

    std::size_t raw_socket::read(char *buf, std::size_t len) {
        return 0;
    }

    std::size_t raw_socket::write(const char *const buf, std::size_t len) {
        return 0;
    }
}