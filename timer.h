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
        typedef std::function<void()>                 timer_call_back;

        timer(const timer_call_back& cb, time_stamp when, double interval = 0.0);

        void run() const;

        time_stamp expiration() const;

        bool repeat() const;

        void restart(const time_stamp& now);

    private:
        timer(const timer&) = delete;
        void operator=(const timer&) = delete;

    private:
        timer_call_back       m_call_back;
        time_stamp            m_expiration;
        const double          m_interval;
        const bool            m_repeat;
    };
}
