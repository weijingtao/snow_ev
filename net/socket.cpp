//
// Created by weitao on 7/16/16.
//

#include "socket.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <cassert>
#include <cstring>
#include "tcp_socket.h"
#include "udp_socket.h"
#include "logger/logger.h"

namespace snow
{
    std::shared_ptr<socket> socket::create(int domain, int type, int protocol) {
        assert((domain == AF_INET) || (domain == AF_INET6));
        assert((type == ::SOCK_STREAM) || (type == ::SOCK_DGRAM) || (type == ::SOCK_RAW));

        int socket_fd = ::socket(domain, type, protocol);
        SNOW_LOG_DEBUG << "create socket, fd :" << socket_fd;
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

    socket::socket(socket&& rhs)
        : m_fd(rhs.m_fd),
          m_local_endpoint(std::move(rhs.m_local_endpoint)),
          m_peer_endpoint(std::move(rhs.m_peer_endpoint)) {
        rhs.m_fd = -1;
    }

/*    void socket::operator=(socket&& rhs) {
        m_fd = rhs.m_fd;
//        m_local_endpoint(std::move(rhs.m_local_endpoint));
//        m_peer_endpoint(std::move(rhs.m_peer_endpoint));
        rhs.m_fd = -1;
    }*/

    socket::~socket() {
        if(m_fd >= 0) {
            ::close(m_fd);
            m_fd = -1;
        }
    }

    void socket::enable_nonblock() {
        int flags = fcntl(m_fd, F_GETFL, 0);
        fcntl(m_fd, F_SETFL, flags | O_NONBLOCK);
    }

    void socket::enable_reuse_addr() {
        int reuse_addr = 1;
        if(0 != setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&reuse_addr), sizeof(reuse_addr))) {
            SNOW_LOG_FATAL << "socket fd :" << m_fd << " errno:" << errno << ", errmsg:" << ::strerror(errno);
        }
    }

    void socket::enable_reuse_port() {

    }

    void socket::set_send_buf_len(int len) {
        if(0 != setsockopt(m_fd, SOL_SOCKET, SO_SNDBUF, reinterpret_cast<char*>(&len), sizeof(len))) {
            SNOW_LOG_FATAL << "errno:" << errno << ", errmsg:" << ::strerror(errno);
        }
    }

    void socket::set_recv_buf_len(int len) {
        if(0 != setsockopt(m_fd, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<char*>(&len), sizeof(len))) {
            SNOW_LOG_FATAL << "errno:" << errno << ", errmsg:" << ::strerror(errno);
        }
    }

    bool socket::bind(const endpoint &addr) {
        if(0 == ::bind(m_fd, &addr.sockaddr(), sizeof(addr.sockaddr()))) {
            return true;
        } else {
            SNOW_LOG_FATAL << "bind failed fd:" << m_fd << " errno:" << errno << " errmsg:" << strerror(errno);
            return false;
        }
    }
}