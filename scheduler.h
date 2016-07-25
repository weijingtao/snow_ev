//
// Created by weitao on 7/1/16.
//

#pragma once

#include <memory>
#include <functional>
#include <deque>
#include "event/poller.h"
#include "timer/timer_queue.h"
#include "awakener.h"

namespace snow {

    class scheduler {
    public:
        typedef std::function<void()> task_type;
        typedef std::unique_ptr<task_type> task_type_ptr;
        typedef std::function<void()> call_back_before_loop_type;
        typedef std::function<void()> call_back_after_loop_type;

        static scheduler &instance();

        scheduler();

        void start();

        void stop();

        void post(task_type_ptr &task);

        void spawn(task_type_ptr &task);

        std::shared_ptr<poller> get_poller() {
            return  m_poller;
        }

        void set_call_back_before_loop(call_back_before_loop_type cb) {
            m_cb_before_loop = std::move(cb);
        }

        void set_call_back_after_loop(call_back_after_loop_type cb) {
            m_cb_after_loop = std::move(cb);
        }


    private:

        void wake_up();

        void run();

    private:
        std::shared_ptr<poller>      m_poller;
        std::unique_ptr<timer_queue> m_timer_queue;
        std::unique_ptr<awakener>    m_awakener;
        call_back_before_loop_type   m_cb_before_loop;
        call_back_after_loop_type    m_cb_after_loop;
        std::deque<std::unique_ptr<task_type>> m_task_queue;
        bool m_running;
    };

}
