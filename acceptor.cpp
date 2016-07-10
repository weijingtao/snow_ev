//
// Created by weitao on 7/1/16.
//

#include "acceptor.h"

#include <arpa/inet.h>
#include <errno.h>
#include <cassert>
#include <cstring>
#include "scheduler.h"
#include "logger.h"


namespace snow {

    acceptor::acceptor()
        : acceptor("", 0) {

    }

    acceptor::acceptor(const std::string &ip, uint16_t port)
        : m_event(new event),
          m_ip(ip),
          m_port(0),
          m_socket_fd(-1) {
    }

    acceptor::~acceptor() {
        ::close(m_socket_fd);
        m_socket_fd = -1;
    }

    int acceptor::init() {
        struct sockaddr_in bind_addr = {0};
        bind_addr.sin_family = AF_INET;
        bind_addr.sin_port = ::htons(m_port);
        if (::inet_pton(AF_INET, m_ip.c_str(), &bind_addr.sin_addr) < 0) {
//            SNOW_LOG_FATAL << "errno[" << ::errno << "]: " << strerror(::errno);
            return -1;
        }
        m_socket_fd = ::socket(AF_INET, SOCK_STREAM, 0);
        if (m_socket_fd < 0) {
//            perror("errno[%d]:%s", ::errno, strerror(::errno));
            return -1;
        }
        if (::bind(m_socket_fd, reinterpret_cast<sockaddr*>(&bind_addr), sizeof(bind_addr)) < 0) {
//            perror("errno[%d]:%s", ::errno, strerror(::errno));
            return -1;
        }
        if (::listen(m_socket_fd, 10) < 0) {
//            perror("errno[%d]:%s", ::errno, strerror(::errno));
            return -1;
        }
        return 0;
    }

    void acceptor::set_new_connection_handle(new_connection_handle_type &handle) {
        m_new_connection_handle = handle;
    }

    bool acceptor::try_lock() {
        return m_mutex.try_lock();
    }

    void acceptor::unlock() {
        m_mutex.unlock();
    }

    void acceptor::enable_event_call_back() {
        m_event->set_poller(&scheduler::instance().get_poller());
        m_event->set_read_cb(std::bind(&acceptor::handle_read, this));
        m_event->enable_reading();
        m_event->enable_oneshot();
    }

    void acceptor::handle_read() {
        int client_fd = 0;
        socklen_t addr_len = 0;
        struct sockaddr peer_addr = {0};
        do {
            client_fd = ::accept4(m_socket_fd, &peer_addr, &addr_len, ::SOCK_CLOEXEC | ::SOCK_NONBLOCK);
            if (client_fd > 0) {
                if(m_new_connection_handle)
                    m_new_connection_handle(client_fd, peer_addr, addr_len);
            } else {
                if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                    break;
                }
                if (errno == EINTR) {
                    continue;
                }
            }
        } while (true);
    }
}