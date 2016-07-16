//
// Created by weitao on 7/16/16.
//

#include "socket.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cassert>
#include "tcp_socket.h"
#include "udp_socket.h"

namespace snow
{
    std::shared_ptr<socket> socket::create(int domain, int type, int protocol) {
        assert((domain == AF_INET) || (domain == AF_INET6));
        assert((type == ::SOCK_STREAM) || (type == ::SOCK_DGRAM) || (type == ::SOCK_RAW));

        int socket_fd = ::socket(domain, type, protocol);
        switch(type) {
            case ::SOCK_STREAM:
                return std::make_shared<tcp_socket>(socket_fd);
            case ::SOCK_DGRAM:
                return std::make_shared<udp_socket>(socket_fd);

        }
    }

    socket::socket(int fd)
        : m_fd(fd) {
        assert(m_fd >= 0);
    }

    socket::~socket() {
        if(m_fd >= 0) {
            ::close(m_fd);
            m_fd = -1;
        }
    }
}