//
// Created by weitao on 7/16/16.
//

#pragma once

#include "socket.h"

#include <vector>

namespace snow
{
    class tcp_socket : public socket
    {
    public:
        tcp_socket(int fd);

        virtual std::size_t read(buffer* buf);

        virtual std::size_t write(buffer& buf);

        bool listen();

        void accept(std::vector<tcp_socket>* sockets);
    };
}
