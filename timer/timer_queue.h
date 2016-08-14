//
// Created by weitao on 7/9/16.
//

#pragma once

#include <cassert>
#include <map>
#include <set>
#include <vector>
#include <memory>
#include <chrono>
#include <functional>
#include <iterator>
#include "timer.h"
#include "timer_fd.h"
#include "event/poller.h"
#include "../logger/logger.h"

namespace snow
{
    class poller;
    class timer_queue
    {
        class timer_comp
        {
        public:
            bool operator()(const std::reference_wrapper<timer>& lhs, const std::reference_wrapper<timer>& rhs) {
                SNOW_LOG_DEBUG << "lhs " << &lhs.get() << " rhs " << &rhs.get();
                return (&lhs.get() < &rhs.get());
            }
        };
    public:
        typedef std::chrono::steady_clock::time_point time_stamp;
        typedef std::function<void()>                 timer_call_back;
        typedef std::reference_wrapper<timer>         entry;
        typedef std::set<entry, timer_comp>           timer_set;
        typedef timer_set::iterator                   timer_id;


        explicit timer_queue(poller* poller);

        timer_id add_timer(const entry& timer);

        void cancel(timer_id& id);

    private:
        timer_queue(const timer_queue&) = delete;
        void operator=(const timer_queue&) = delete;

        void handle_timeout();

        // move out all expired timers
        std::vector<entry> get_expired(const time_stamp& now);

        void reset(std::vector<entry>& expired, const time_stamp& now);

        bool insert(entry& timer);

    private:
        timer_fd  m_timer_fd;
        timer_set m_timers;
    };
}
