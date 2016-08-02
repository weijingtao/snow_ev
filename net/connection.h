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
    class buffer;
    class scheduler;

    class connection
    {
    public:
        typedef std::unique_ptr<connection> connection_ptr_type;
        typedef std::list<connection_ptr_type>::const_iterator index_type;
        typedef std::function<void(index_type&)> close_handle_type;
        typedef std::function<void(const buffer&)> response_handle_type;
        typedef std::function<void(const char* data, std::size_t len, response_handle_type)> dispatcher_type;
        typedef std::function<std::size_t(const char*const, std::size_t)> pkg_split_type;


        connection(tcp_socket&& socket);

        ~connection();

        void start();

        void stop();

        void set_dispatcher(dispatcher_type&& dispatcher) {
            m_dispatcher = std::move(dispatcher);
        }

        void set_pkg_spliter(pkg_split_type&& pkg_split) {
            m_pkg_spliter = std::move(pkg_split);
        }

        void set_index(const index_type& index) {
            m_index = index;
        }

        void set_close_handler(const close_handle_type& handler) {
            m_close_handler = handler;
        }

        void set_timeout(std::size_t timeout) {
            m_timeout = timeout;
        }

    private:
        connection(const connection&) = delete;
        void operator=(const connection&) = delete;

        void send(const buffer& data);

        void handle_read();

        void handle_write();

        void handle_timeout();

        void handle_close();

    private:
        tcp_socket             m_socket;
        event                  m_io_event;
        timer                  m_timer;
        dispatcher_type        m_dispatcher;
        pkg_split_type         m_pkg_spliter;
        buffer                 m_recv_buffer;
        buffer                 m_send_buffer;
        index_type             m_index;
        close_handle_type      m_close_handler;
        std::size_t            m_timeout;
    };
}
