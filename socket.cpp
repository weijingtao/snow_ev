//
// Created by weitao on 7/16/16.
//

#include "socket.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cassert>

namespace snow
{
    std::shared_ptr<socket> socket::create(int domain, int type, int protocol) {
        int socket_fd = ::socket(domain, type, protocol);
        return std::make_shared<socket>(socket_fd);
    }

    socket::socket(int32_t fd)
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