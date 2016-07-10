//
// Created by weitao on 7/9/16.
//

#include "timer_fd.h"
#include <sys/timerfd.h>
#include <cstring>
#include "poller.h"

namespace snow
{
    timer_fd::timer_fd(std::shared_ptr<poller>& poller1)
        : m_event(poller1),
          m_fd(-1) {

    }

    timer_fd::~timer_fd() {
        m_event.disable_all();
        ::close(m_fd);
        m_fd = -1;
    }

    int timer_fd::init() {
        m_fd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
        if(m_fd < 0)
            return -1;
        m_event.set_read_cb(std::bind(&timer_fd::handle_read, this));
        m_event.enable_reading();
        return 0;
    }

    void timer_fd::set_timeout_cb(time_out_cb cb) {
        m_cb = std::move(cb);
    }

    struct timespec timer_fd::how_much_time_from_now(time_stamp& when) {
        /*int64_t microseconds = when.microSecondsSinceEpoch()
                               - Timestamp::now().microSecondsSinceEpoch();
        if (microseconds < 100)
        {
            microseconds = 100;
        }*/
        struct timespec ts;
        /*ts.tv_sec = static_cast<time_t>(
            microseconds / Timestamp::kMicroSecondsPerSecond);
        ts.tv_nsec = static_cast<long>(
            (microseconds % Timestamp::kMicroSecondsPerSecond) * 1000);*/
        return ts;
    }

    void timer_fd::handle_read() {
        uint64_t how_many;
        ssize_t n = ::read(m_fd, &how_many, sizeof how_many);
//            LOG_TRACE << "TimerQueue::handleRead() " << howmany << " at " << now.toString();
        if (n != sizeof how_many) {
//                LOG_ERROR << "TimerQueue::handleRead() reads " << n << " bytes instead of 8";
        }
    }

    void timer_fd::reset(time_stamp& expiration) {
        // wake up loop by timerfd_settime()
        struct itimerspec newValue;
        struct itimerspec oldValue;
        std::memset(&newValue, 0, sizeof newValue);
        std::memset(&oldValue, 0, sizeof oldValue);
        newValue.it_value = how_much_time_from_now(expiration);
        int ret = ::timerfd_settime(m_fd, 0, &newValue, &oldValue);
        if (ret) {
//            LOG_SYSERR << "timerfd_settime()";
        }
    }

}