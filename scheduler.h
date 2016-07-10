//
// Created by weitao on 7/1/16.
//

#pragma once


#include <memory>
#include <functional>
#include <deque>
#include "poller.h"
#include "timer_queue.h"

namespace snow {
    class task;

    class scheduler {
    public:
        typedef std::function<void()> task_type;
        typedef std::unique_ptr<task_type> task_type_ptr;

        static scheduler &instance();

        scheduler();

        void init();

        void start();

        void stop();

        void post(task_type_ptr &task);

        void spawn(task_type_ptr &task);

        poller& get_poller() {
        }


    private:

        void wake_up();

        void run();

    private:
        std::unique_ptr<poller> m_poller;
        std::unique_ptr<timer_queue> m_timer_queue;
        std::deque<std::unique_ptr<task_type>> m_task_queue;
        int m_socket_pair[2];
        bool m_running;
    };

}
