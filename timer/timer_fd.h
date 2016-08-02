//
// Created by weitao on 7/9/16.
//

#pragma once

#include <functional>
#include <chrono>
#include "event/poller.h"
#include "event/event.h"

namespace snow
{
    class poller;

    class timer_fd
    {
    public:
        typedef std::chrono::steady_clock::time_point time_stamp;
        typedef std::function<void(void)> time_out_cb;

        timer_fd(poller* poller1);

        ~timer_fd();

        int init();

        void set_timeout_cb(time_out_cb cb);

        timespec how_much_time_from_now(const time_stamp& when);

        void handle_read();

        void reset(const time_stamp& expiration);

    private:
        event   m_event;
        time_out_cb m_cb;
        int     m_fd;
    };
}

