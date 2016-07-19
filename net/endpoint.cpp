//
// Created by weitao on 7/16/16.
//

#include "endpoint.h"


#include <cstring>

namespace snow
{
    endpoint::endpoint(const struct sockaddr &addr)
        : m_addr(addr) {

    }

    endpoint::endpoint(const std::string &ip, uint16_t port)
        : m_addr{0} {
        if(std::string::npos == ip.find(':')) {
            //ipv4
            sockaddr_in* addr = reinterpret_cast<sockaddr_in*>(&m_addr);
            if(1 == ::inet_pton(AF_INET, ip.c_str(), &(addr->sin_addr))) {
                addr->sin_family  = AF_INET;
                addr->sin_port    = ::htons(port);
                m_valide = true;
            }
        } else {
            //ipv6
            sockaddr_in6* addr = reinterpret_cast<sockaddr_in6*>(&m_addr);
            if(1 == ::inet_pton(AF_INET6, ip.c_str(), &(addr->sin6_addr))) {
                addr->sin6_family = AF_INET6;
                addr->sin6_port   = ::htons(port);
                m_valide = true;
            }
        }
    }

    endpoint::operator bool() const {
        return m_valide;
    }

    const sockaddr & endpoint::sockaddr() const {
        return m_addr;
    }
}