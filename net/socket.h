//
// Created by weitao on 7/16/16.
//

#pragma once

#include <memory>
#include "endpoint.h"

struct sockaddr;

namespace snow
{
    class socket
    {
    public:
        static std::shared_ptr<socket> create(int domain, int type, int protocol);

        explicit socket(int fd);

        virtual ~socket();

        void set_local_endpoint(const std::shared_ptr<endpoint>& local_endpoint) {
            m_local_endpoint = local_endpoint;
        }

        void set_local_endpoint(std::shared_ptr<endpoint>&& local_endpoint) {
            m_local_endpoint = std::move(local_endpoint);
        }

        const std::shared_ptr<endpoint>& get_local_endpoint() const {
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


    private:
        int m_fd;
        std::shared_ptr<endpoint> m_local_endpoint;
        endpoint                  m_peer_endpoint;
    };
}
