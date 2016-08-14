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
        : m_socket(::socket(addr.is_v4() ? AF_INET : AF_INET6, SOCK_STREAM, 0)) {
        m_socket.enable_nonblock();
        m_socket.enable_reuse_addr();
        assert(m_socket.bind(addr));
        assert(m_socket.listen());
        m_event.set_read_cb(std::bind(&acceptor::handle_read, this));
        m_event.set_socket_fd(m_socket.get_socket_fd());
        SNOW_LOG_DEBUG << "acceptor socket fd:" << m_socket.get_socket_fd();
    }

    void acceptor::set_new_connection_handle(const new_connection_handle_type& handle) {
        m_new_connection_handle = std::move(handle);
    }

    bool acceptor::try_lock() {
        SNOW_LOG_DEBUG << "acceptor event addr " << &m_event;
        return m_mutex.try_lock();
    }

    void acceptor::unlock() {
        m_mutex.unlock();
    }

    void acceptor::enable_event_call_back() {
        m_event.set_poller(&scheduler::instance().get_poller());
        m_event.enable_reading();
        m_event.enable_oneshot();
    }

    void acceptor::handle_read() {
        std::vector<tcp_socket> sockets;
        m_socket.accept(&sockets);
        if(m_new_connection_handle && !sockets.empty()) {
            for(auto& socket : sockets) {
                SNOW_LOG_DEBUG << "new socket fd:" << socket.get_socket_fd();
                std::unique_ptr<connection> new_connection(new connection(std::move(socket)));
                m_new_connection_handle(new_connection);
            }
        }
        unlock();
    }
}