//
// Created by weitao on 7/9/16.
//

//
// Created by weitao on 4/1/16.
//
#include "timer_queue.h"
#include <cstring>

namespace snow
{
    timer_queue::timer_queue(poller* poller)
      : m_timer_fd(poller) {
        m_timer_fd.init();
        m_timer_fd.set_timeout_cb(std::bind(&timer_queue::handle_timeout, this));
    }

    timer_queue::timer_id timer_queue::add_timer(const entry& timer) {
        bool earliestChanged = false;
        auto when = timer.get().expiration();
        auto it   = m_timers.cbegin();
        if (it == m_timers.cend() || when < it->get().expiration()) {
            earliestChanged = true;
        }
        auto result = m_timers.insert(timer);
        assert(result.second);
        if(earliestChanged) {
            m_timer_fd.reset(m_timers.cbegin()->get().expiration());
        }

        return result.first;
    }

    void timer_queue::cancel(timer_id& id) {
        if(id == m_timers.begin()) {
            m_timers.erase(id);
            m_timer_fd.reset(m_timers.cbegin()->get().expiration());
        } else {
            m_timers.erase(id);
        }
    }

    void timer_queue::handle_timeout() {
        auto now = std::chrono::steady_clock::now();
        std::vector<entry> expired = get_expired(now);
        for (auto& timer : expired) {
            timer.get().run();
        }
        reset(expired, now);
    }

    // move out all expired timers
    std::vector<timer_queue::entry> timer_queue::get_expired(const time_stamp& now) {
        std::vector<entry> expired;
        for(auto it = m_timers.begin(); it != m_timers.end();) {
            if(it->get().expiration() <= now) {
                expired.push_back(*it);
                it = m_timers.erase(it);
            }
            break;
        }
        /*timer sentry;
        auto end = m_timers.lower_bound(std::ref(sentry));
        std::copy(m_timers.begin(), end, std::back_inserter(expired));
        m_timers.erase(m_timers.begin(), end);*/
        return std::move(expired);
    }

    void timer_queue::reset(std::vector<entry>& expired, const time_stamp& now) {
        for (auto& timer : expired) {
            if(timer.get().repeat()) {
                timer.get().restart(now);
                insert(timer);
            }
        }

        if (!m_timers.empty()) {
            m_timer_fd.reset(m_timers.cbegin()->get().expiration());
        }
    }

    bool timer_queue::insert(entry& timer) {
        bool earliestChanged = false;
        auto when = timer.get().expiration();
        auto it   = m_timers.cbegin();
        if (it == m_timers.cend() || when < it->get().expiration()) {
            earliestChanged = true;
        }
        auto result = m_timers.insert(timer);
        assert(result.second);

        return earliestChanged;
    }

}