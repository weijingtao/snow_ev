//
// Created by weitao on 7/9/16.
//

#pragma once

#include <memory>
#include <functional>
#include <chrono>

namespace snow
{
    class timer
    {
    public:
        typedef std::chrono::steady_clock::time_point time_stamp;
        typedef std::function<void()>                 timeout_handler_type;

        timer() = default;

        timer(const timeout_handler_type& cb, time_stamp when, std::chrono::milliseconds interval);

        void run() const;

        void set_timeout_handler(const timeout_handler_type& handler) {
            m_timeout_handler = handler;
        }

        void set_time_stamp(const time_stamp& stamp) {
            m_expiration = stamp;
        }

        void set_interval(std::chrono::milliseconds interval) {
            m_interval = interval;
        }

        time_stamp expiration() const;

        bool repeat() const;

        void restart(const time_stamp& now);

        void cancel();

    private:
        timer(const timer&) = delete;
        void operator=(const timer&) = delete;

    private:
        timeout_handler_type      m_timeout_handler;
        time_stamp                m_expiration;
        std::chrono::milliseconds m_interval;
    };
}
