//
// Created by weitao on 7/1/16.
//

#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdint>
#include <string>
#include <functional>
#include <memory>
#include <mutex>
#include "../event/event.h"
#include "endpoint.h"
#include "tcp_socket.h"
#include "connection.h"

namespace snow {
    class acceptor {
    public:
        typedef std::function<void(std::unique_ptr<connection>&)> new_connection_handle_type;

        acceptor(const endpoint& addr);

        ~acceptor() = default;

        void set_new_connection_handle(const new_connection_handle_type& handle);

        bool try_lock();

        void unlock();

        void enable_event_call_back();

    private:
        acceptor(const acceptor &) = delete;
        void operator=(const acceptor &) = delete;

        void handle_read();

    private:
        new_connection_handle_type m_new_connection_handle;
        event                      m_event;
        std::mutex                 m_mutex;
        tcp_socket                 m_socket;
    };
}
