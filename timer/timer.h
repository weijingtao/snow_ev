//
// Created by weitao on 7/9/16.
//

#pragma once

#include <memory>
#include <functional>
#include <chrono>
#include <set>

namespace snow
{
    class scheduler;

    class timer
    {
    public:
        typedef std::chrono::steady_clock::time_point time_stamp;
        typedef std::function<void()>                 timeout_handler_type;
        typedef std::reference_wrapper<timer>         entry;
        class timer_comp
        {
        public:
            bool operator()(const entry& lhs, const entry& rhs) {
                return (lhs.get() < rhs.get());
            }
        };

        typedef std::set<entry, timer_comp>           timer_set;
        typedef timer_set::iterator                   timer_id;

        timer()
            : m_running(false) {

        }

        timer(const timeout_handler_type& cb, time_stamp when, std::chrono::milliseconds interval);

        void start();

        void run();

        void set_timeout_handler(const timeout_handler_type& handler) {
            m_timeout_handler = handler;
        }

        void set_time_stamp(const time_stamp& stamp) {
            m_expiration = stamp;
        }

        void set_interval(std::chrono::milliseconds interval) {
            m_interval = interval;
        }

        void set_index(const timer_id& id) {
            m_id = id;
        }

        time_stamp expiration() const;

        bool repeat() const;

        void restart(const time_stamp& now);

        void cancel();

        bool operator<(const timer&);

    private:
        timer(const timer&) = delete;
        void operator=(const timer&) = delete;

    private:
        timeout_handler_type      m_timeout_handler;
        time_stamp                m_expiration;
        std::chrono::milliseconds m_interval;
        timer_id                  m_id;
        bool                      m_running;
    };
}
