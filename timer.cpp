//
// Created by weitao on 7/9/16.
//

//
// Created by weitao on 4/1/16.
//

#include "timer.h"

namespace snow
{
    timer::timer(const timer_call_back& cb, time_stamp when, double interval)
        : m_callback(std::move(cb)),
          m_expiration(when),
          m_interval(interval),
          m_repeat(interval > 0.0)
    { }

    void timer::run() const
    {
        m_call_back();
    }

    time_stamp timer::expiration() const  { return m_expiration; }

    bool timer::repeat() const { return m_repeat; }

    void timer::restart(const time_stamp& now) {
        m_expiration = now + std::chrono::duration_cast<std::chrono::duration<double>>(const_cast<double>(_interval));
    }
}