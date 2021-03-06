//
// Created by weitao on 7/16/16.
//

#pragma once

#include "socket.h"

namespace snow
{
    class udp_socket : public socket
    {
    public:
        udp_socket(int fd);

        virtual std::size_t read(buffer*);

        virtual std::size_t write(buffer&);
    };
}
