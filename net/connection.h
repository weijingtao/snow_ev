//
// Created by weitao on 7/2/16.
//

#pragma once

#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory>
#include <functional>
#include "event/event.h"
#include "timer/timer.h"
#include "buffer.h"
#include "tcp_socket.h"

namespace snow
{
    class request;
    class buffer;
    class connection
    {
    public:
        typedef std::function<void(const buffer&)> dispatcher_type;
        typedef std::function<std::size_t(const char*const, std::size_t)> pkg_split_type;

        explicit connection(tcp_socket& socket);

        ~connection();

        void start();

        void stop();

        void send(const buffer& data);

        void set_dispatcher(dispatcher_type&& dispatcher) {
            m_dispatcher = std::move(dispatcher);
        }

        void set_pkg_spliter(pkg_split_type&& pkg_split) {
            m_pkg_spliter = std::move(pkg_split);
        }

    private:
        void handle_read();

        void handle_write();

        void handle_timeout();

    private:
        tcp_socket             m_socket;
        event                  m_io_event;
        timer                  m_timer;
        dispatcher_type        m_dispatcher;
        pkg_split_type         m_pkg_spliter;
        buffer                 m_recv_buffer;
        buffer                 m_send_buffer;
    };
}
