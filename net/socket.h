//
// Created by weitao on 7/16/16.
//

#pragma once

#include <cstdint>
#include <memory>
#include <locale>
#include "endpoint.h"
#include "buffer.h"

struct sockaddr;

namespace snow
{
    class socket
    {
    public:
        static std::shared_ptr<socket> create(int domain, int type, int protocol);

        explicit socket(int fd);

        socket(socket&&);

//        void operator=(socket&&);

        virtual ~socket();

        void set_socket_fd(int fd) {
            m_fd = fd;
        }

        int get_socket_fd() const {
            return m_fd;
        }

        void set_local_endpoint(const endpoint& local_endpoint) {
            m_local_endpoint = local_endpoint;
        }

        void set_local_endpoint(endpoint&& local_endpoint) {
            m_local_endpoint = std::move(local_endpoint);
        }

        const endpoint& get_local_endpoint() const {
            return m_local_endpoint;
        }

        void set_peer_endpoint(const endpoint& peer_endpoint) {
            m_peer_endpoint = peer_endpoint;
        }

        void set_peer_endpoint(endpoint&& peer_endpoint) {
            m_peer_endpoint = std::move(peer_endpoint);
        }

        const endpoint& get_peer_endpoint() const {
            return m_peer_endpoint;
        }

        virtual std::size_t read(buffer* buf) = 0;

        virtual std::size_t write(buffer& buf) = 0;

        void enable_nonblock();

        void enable_reuse_port();

        void enable_reuse_addr();

        void set_send_buf_len(int len);

        void set_recv_buf_len(int len);

        bool bind(const endpoint& addr);

    private:
        socket(const socket&) = delete;
        void operator=(const socket&) = delete;


    protected:
        int      m_fd;
        endpoint m_local_endpoint;
        endpoint m_peer_endpoint;
    };
}
