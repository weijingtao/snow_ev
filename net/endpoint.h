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


    private:
        struct sockaddr m_addr;
        bool     m_valide;
    };
}
