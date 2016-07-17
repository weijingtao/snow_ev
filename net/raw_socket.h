//
// Created by weitao on 7/16/16.
//

#pragma once

#include "socket.h"

namespace snow
{
    class raw_socket : public socket
    {
    public:
        raw_socket(int fd);

        virtual std::size_t read(char* buf, std::size_t len);

        virtual std::size_t write(const char* const buf, std::size_t len);
    };
}
