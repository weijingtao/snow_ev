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
    };
}
