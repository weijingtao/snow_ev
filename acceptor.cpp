//
// Created by weitao on 7/1/16.
//

#include "acceptor.h"

#include <arpa/inet.h>
#include <cassert>
#include <cstring>
#include <ev.h>
#include "scheduler.h"

acceptor::acceptor()
  : acceptor("", 0) {

}

acceptor::acceptor(const std::string &ip, uint16_t port)
  : m_io_watcher(new ev_io),
    m_ip(ip),
    m_port(0),
    m_socket_fd(-1) {
    std::memset(m_io_watcher.get(), 0, sizeof(*m_io_watcher.get()));
}

acceptor::acceptor(acceptor &&rhs)
  : acceptor() {
    swap(rhs);
}

acceptor::~acceptor() {
    ::close(m_socket_fd);
    m_socket_fd = -1;
}

void acceptor::operator=(acceptor &&rhs) {
    acceptor temp;
    swap(temp);
    swap(rhs);
}

int acceptor::init() {
    struct sockaddr_in bind_addr = {0};
    bind_addr.sin_family = AF_INET;
    bind_addr.sin_port   = ::htons(m_port);
    if(::inet_pton(AF_INET, m_ip.c_str(), bind_addr.sin_addr) < 0) {
        perror("errno[%d]:%s", ::errno, strerror(::errno));
        return -1;
    }
    m_socket_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if(m_socket_fd < 0) {
        perror("errno[%d]:%s", ::errno, strerror(::errno));
        return -1;
    }
    if(::bind(m_socket_fd, &bind_addr, sizeof(bind_addr)) < 0) {
        perror("errno[%d]:%s", ::errno, strerror(::errno));
        return -1;
    }
    if(::listen(m_socket_fd, 10) < 0) {
        perror("errno[%d]:%s", ::errno, strerror(::errno));
        return -1;
    }
    return 0;
}

void acceptor::handle_new_connection() {
    int client_fd      = 0;
    socklen_t addr_len = 0;
    struct sockaddr peer_addr = {0};
    do {
        client_fd = ::accept4(m_socket_fd, &peer_addr, &addr_len, ::SOCK_CLOEXEC|::SOCK_NONBLOCK);
        if(client_fd > 0) {
            m_new_connection_handle(client_fd, peer_addr, addr_len);
        } else {
            if((::errno == EAGAIN) || (::errno == EWOULDBLOCK)) {
                break;
            }
            if(::errno == EINTR) {
                continue;
            }
        }
    } while (true);
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
    ev_io_init(m_io_watcher.get(), &acceptor::handle_read, m_socket_fd, EV_READ);
    ev_io_start(scheduler::instance().event_loop(), m_io_watcher.get());
}

void acceptor::handle_read(ev_loop *loop, ev_io *io_watcher, int revents) {
    auto* acceptor_ptr = static_cast<acceptor*>(ev_userdata(io_watcher));
    acceptor_ptr->handle_new_connection();
}

void acceptor::swap(acceptor &rhs) {
    using std::swap;
    swap(m_io_watcher, rhs.m_io_watcher);
    swap(m_ip, rhs.m_ip);
    swap(m_port, rhs.m_port);
    swap(m_new_connection_handle, rhs.m_new_connection_handle);
    swap(m_mutex, rhs.m_mutex);
    swap(m_socket_fd, rhs.m_socket_fd);
}