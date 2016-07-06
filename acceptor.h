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
#include <ev.h>

namespace snow {
    class acceptor {
    public:
        typedef std::function<void(int, const struct sockaddr &, int)> new_connection_handle_type;

        acceptor();

        acceptor(const std::string &ip, uint16_t port);

        acceptor(acceptor &&rhs);

        ~acceptor();

        void operator=(acceptor &&rhs);

        int init();

        void handle_new_connection();

        void set_new_connection_handle(new_connection_handle_type &handle);

        bool try_lock();

        void unlock();

        void enable_event_call_back();

    private:
        static void handle_read(struct ev_loop *loop, ev_io *io_watcher, int revents);

        acceptor(const acceptor &) = delete;

        void operator=(const acceptor &) = delete;

        void swap(acceptor &rhs);

    private:
        std::unique_ptr<ev_io> m_io_watcher;
        std::string m_ip;
        uint16_t m_port;
        new_connection_handle_type m_new_connection_handle;
        std::mutex m_mutex;
        int m_socket_fd;
    };
}
