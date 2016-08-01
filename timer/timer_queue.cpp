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
    timer_queue::timer_queue(std::shared_ptr<poller>& poller)
      : m_timer_fd(poller) {
        m_timer_fd.init();
        m_timer_fd.set_timeout_cb(std::bind(&timer_queue::handle_timeout, this));
    }

    ///
    /// Schedules the callback to be run at given time,
    /// repeats if @c interval > 0.0.
    ///
    /// Must be thread safe. Usually be called from other threads.
    timer_queue::timer_id timer_queue::add_timer(timer_call_back& cb,
                                    time_stamp when,
                                                 std::chrono::milliseconds interval) {
        auto new_timer = std::make_shared<timer>(cb, when, interval);
        return new_timer;
    }

    void timer_queue::cancel(timer_id& id) {
        /*size_t n = _timers.erase(id);
        assert(n == 1);*/
    }

    void timer_queue::handle_timeout() {
        auto now = std::chrono::steady_clock::now();
        std::vector<entry> expired = get_expired(now);
        for (auto& timer : expired) {
            timer.second->run();
        }
        reset(expired, now);
    }

    // move out all expired timers
    std::vector<timer_queue::entry> timer_queue::get_expired(const time_stamp& now) {
        std::vector<entry> expired;
        entry sentry(now, std::shared_ptr<timer>());
        auto end = m_timers.lower_bound(sentry);
        std::copy(m_timers.begin(), end, std::back_inserter(expired));
        m_timers.erase(m_timers.begin(), end);
        return std::move(expired);
    }

    void timer_queue::reset(std::vector<entry>& expired, const time_stamp& now) {
        for (auto& timer : expired) {
            if(timer.second->repeat()) {
                timer.second->restart(now);
                insert(timer.second);
            }
        }

        time_stamp next_expire = now ;//+ std::chrono::duration_cast<std::chrono::duration<double>>(1000.0);

        if (!m_timers.empty()) {
            next_expire = m_timers.cbegin()->second->expiration();
        }
        m_timer_fd.reset(next_expire);
    }

    bool timer_queue::insert(std::shared_ptr<timer>& timer) {
        bool earliestChanged = false;
        auto when = timer->expiration();
        auto it   = m_timers.cbegin();
        if (it == m_timers.cend() || when < it->first) {
            earliestChanged = true;
        }
        auto result = m_timers.insert(entry(when, timer));
        assert(result.second);

        return earliestChanged;
    }


}