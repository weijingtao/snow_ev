//
// Created by weitao on 7/2/16.
//

#pragma once

#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory>
#include <functional>
#include "event.h"
#include "timer.h"
#include "buffer.h"

void enable_nonblock(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

namespace snow
{
    class request;
    class buffer;
    class connection
    {
    public:
        typedef std::function<void(const buffer&)> dispatcher_type;
        typedef std::function<std::size_t(const char*const, std::size_t)> pkg_split_type;

        explicit connection(int socket_fd);

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
        std::unique_ptr<event> m_io_event;
        std::unique_ptr<timer> m_timer;
        dispatcher_type        m_dispatcher;
        pkg_split_type         m_pkg_spliter;
        buffer                 m_recv_buffer;
        buffer                 m_send_buffer;
        int                    m_socket_fd;
    };
}
