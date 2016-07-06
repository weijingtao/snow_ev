//
// Created by weitao on 7/1/16.
//

#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <memory>
#include <functional>
#include <deque>
#include <ev.h>

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

        struct ev_loop *event_loop() {
            return m_loop.get();
        }


    private:
        static void ev_io_call_back(struct ev_loop *, ev_io *, int);

        static void ev_timer_call_back(struct ev_loop *, ev_timer *);

        void wake_up();

        void run();

    private:
        std::unique_ptr<struct ev_loop> m_loop;
        std::unique_ptr<ev_io> m_io_watcher;
        std::deque<std::unique_ptr<task_type>> m_task_queue;
        int m_socket_pair[2];
        bool m_running;
    };

}
