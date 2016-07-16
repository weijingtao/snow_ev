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
    };
}
