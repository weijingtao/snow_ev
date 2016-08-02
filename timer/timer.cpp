//
// Created by weitao on 7/9/16.
//

#include "timer.h"
#include <functional>
#include "../scheduler.h"

namespace snow
{
    timer::timer(const timeout_handler_type& cb, time_stamp when, std::chrono::milliseconds interval)
        : m_timeout_handler(std::move(cb)),
          m_expiration(when),
          m_interval(interval) {
        m_id = scheduler::instance().get_timer_queue().add_timer(std::ref(*this));
    }

    void timer::start() {
        m_id = scheduler::instance().get_timer_queue().add_timer(std::ref(*this));
    }

    void timer::run() const
    {
        if(m_timeout_handler) {
            m_timeout_handler();
        }
    }

    timer::time_stamp timer::expiration() const  {
        return m_expiration;
    }

    bool timer::repeat() const {
        return (m_interval > std::chrono::milliseconds(0));
    }

    void timer::restart(const time_stamp& now) {
        m_expiration = now + m_interval;
    }

    void timer::cancel() {
        scheduler::instance().get_timer_queue().cancel(m_id);
    }

    bool timer::operator<(const timer& rhs) {
        return (this->m_expiration < rhs.m_expiration);
    }
}