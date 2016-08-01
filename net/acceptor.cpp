//
// Created by weitao on 7/1/16.
//

#include "acceptor.h"

#include <arpa/inet.h>
#include <errno.h>
#include <cassert>
#include <cstring>
#include "scheduler.h"
#include "logger/logger.h"


namespace snow {

    acceptor::acceptor(const endpoint& addr)
        : m_event(new event),
          m_socket(::socket(addr.is_v4() ? AF_INET : AF_INET6, SOCK_STREAM, 0)),
          m_local_addr(addr) {
//        int fd = ::socket(addr.is_v4() ? AF_INET : AF_INET6, SOCK_STREAM, 0);
//        assert(fd >= 0);
//        m_socket.set_socket_fd(fd);
        m_socket.enable_nonblock();
        m_socket.enable_reuse_addr();
        m_socket.bind(m_local_addr);
        m_socket.listen();
    }

    acceptor::~acceptor() {
    }

    int acceptor::init() {
        /*struct sockaddr_in bind_addr = {0};
        bind_addr.sin_family = AF_INET;
        bind_addr.sin_port = ::htons(m_port);
        if (::inet_pton(AF_INET, m_ip.c_str(), &bind_addr.sin_addr) < 0) {
            SNOW_LOG_FATAL << "errno[" << errno << "]: " << strerror(errno);
            return -1;
        }
        m_socket_fd = ::socket(AF_INET, SOCK_STREAM, 0);
        if (m_socket_fd < 0) {
            SNOW_LOG_FATAL << "errno[" << errno << "]: " << strerror(errno);
            return -1;
        }
        if (::bind(m_socket_fd, reinterpret_cast<sockaddr*>(&bind_addr), sizeof(bind_addr)) < 0) {
            SNOW_LOG_FATAL << "errno[" << errno << "]: " << strerror(errno);
            return -1;
        }
        if (::listen(m_socket_fd, 10) < 0) {
            SNOW_LOG_FATAL << "errno[" << errno << "]: " << strerror(errno);
            return -1;
        }*/
        return 0;
    }

    void acceptor::set_new_connection_handle(new_connection_handle_type handle) {
        m_new_connection_handle = std::move(handle);
    }

    bool acceptor::try_lock() {
        return m_mutex.try_lock();
    }

    void acceptor::unlock() {
        m_mutex.unlock();
    }

    void acceptor::enable_event_call_back() {
//        m_event->set_poller(scheduler::instance().get_poller());
        auto& sch = scheduler::instance();
        m_event->set_read_cb(std::bind(&acceptor::handle_read, this));
        m_event->enable_reading();
        m_event->enable_oneshot();
    }

    void acceptor::handle_read() {
        std::vector<tcp_socket> sockets;
        m_socket.accept(&sockets);
        if(m_new_connection_handle && !sockets.empty()) {
            for(auto& socket : sockets) {
                std::unique_ptr<connection> new_connection(new connection(std::move(socket)));
                m_new_connection_handle(new_connection);
            }
        }
    }
}