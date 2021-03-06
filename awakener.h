//
// Created by weitao on 7/10/16.
//

#pragma once

#include "event/event.h"

namespace snow
{
    class poller;

    class awakener
    {
    public:
        explicit awakener(poller* poller1);

        ~awakener();

        void wake_up();

    private:
        awakener(const awakener&) = delete;
        void operator=(const awakener&) = delete;

        void handle_read();

    private:
        event m_event;
        int32_t m_socket_pair[2];
    };
}
