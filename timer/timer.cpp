//
// Created by weitao on 7/9/16.
//

#include "timer.h"

namespace snow
{
    timer::timer()
        : m_interval(0.0),
          m_repeat(false) {

    }

    timer::timer(const timer_call_back& cb, time_stamp when, double interval)
        : m_call_back(std::move(cb)),
          m_expiration(when),
          m_interval(interval),
          m_repeat(interval > 0.0)
    { }

    void timer::run() const
    {
        m_call_back();
    }

    timer::time_stamp timer::expiration() const  {
        return m_expiration;
    }

    bool timer::repeat() const {
        return m_repeat;
    }

    void timer::restart(const time_stamp& now) {
//        m_expiration = now + std::chrono::duration_cast<std::chrono::duration<double>>(const_cast<double>(m_interval));
    }

    void timer::cancel() {

    }
}