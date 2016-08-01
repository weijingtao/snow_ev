//
// Created by weitao on 7/9/16.
//

#include "timer.h"

namespace snow
{
    timer::timer(const timeout_handler_type& cb, time_stamp when, std::chrono::milliseconds interval)
        : m_timeout_handler(std::move(cb)),
          m_expiration(when),
          m_interval(interval) {

    }

    void timer::run() const
    {
        m_timeout_handler();
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

    }
}