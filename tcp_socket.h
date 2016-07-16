//
// Created by weitao on 7/16/16.
//

#pragma once

#include "socket.h"

namespace snow
{
    class tcp_socket : public socket
    {
    public:
        tcp_socket(int fd);
    };
}
