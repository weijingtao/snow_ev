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

namespace snow
{
    class poller;
    class timer_queue
    {
    public:
        typedef std::chrono::steady_clock::time_point time_stamp;
        typedef std::function<void()>                 timer_call_back;
        typedef std::shared_ptr<timer>                timer_id;
        typedef std::pair<time_stamp, std::shared_ptr<timer>> entry;
        typedef std::set<entry>                               timer_set;


        explicit timer_queue(std::shared_ptr<poller>& poller);

        timer_id add_timer(timer_call_back& cb,
                           time_stamp when,
                           double interval);

        void cancel(timer_id& id);

    private:
        timer_queue(const timer_queue&) = delete;
        void operator=(const timer_queue&) = delete;



    private:


        void handle_timeout();

        // move out all expired timers
        std::vector<entry> get_expired(const time_stamp& now);

        void reset(std::vector<entry>& expired, const time_stamp& now);

        bool insert(std::shared_ptr<timer>& timer);

    private:
        timer_fd  m_timer_fd;
        timer_set m_timers;
    };
}
