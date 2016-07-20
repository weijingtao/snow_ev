//
// Created by weitao on 7/16/16.
//

#pragma once

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstdint>
#include <string>

namespace snow
{
    class endpoint
    {
    public:
        endpoint() {}

        explicit endpoint(const struct sockaddr& addr);

        endpoint(const std::string& ip, uint16_t port);

        operator bool() const;

        const struct sockaddr& sockaddr() const;

        bool is_v4() const {
            return true;
        }

        bool is_v6() const {
            return false;
        }


    private:
        struct sockaddr m_addr;
        bool     m_valide;
    };
}
