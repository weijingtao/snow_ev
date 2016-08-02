//
// Created by weitao on 7/10/16.
//

#include "awakener.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include "event/poller.h"

namespace snow
{
    awakener::awakener(poller* poller1)
        : m_event{poller1},
          m_socket_pair{-1, -1} {
        if (0 == ::socketpair(AF_LOCAL, ::SOCK_DGRAM, 0, m_socket_pair)) {
            ::shutdown(m_socket_pair[0], ::SHUT_RD);
            ::shutdown(m_socket_pair[1], ::SHUT_WR);

            m_event.set_read_cb(std::bind(&awakener::handle_read, this));
            m_event.enable_reading();
        }
    }

    awakener::~awakener() {
        m_event.disable_all();
        ::close(m_socket_pair[0]);
        m_socket_pair[0] = -1;
        ::close(m_socket_pair[1]);
        m_socket_pair[1] = -1;
    }

    void awakener::wake_up() {
        while(::write(m_socket_pair[0], "#", 1) < 0);
    }


    void awakener::handle_read() {
        std::array<char, 32> buffer;
        std::size_t n_read = 0;
        do {
            n_read = ::read(m_socket_pair[1], buffer.data(), buffer.size());
        } while (n_read > 0 || errno == EINTR);
    }
}