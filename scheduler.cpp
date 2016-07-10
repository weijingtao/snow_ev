//
// Created by weitao on 7/1/16.
//

#include "scheduler.h"


#include <cassert>
#include <thread>
#include <array>
#include "event.h"

namespace snow {

    scheduler &scheduler::instance() {
        static thread_local scheduler the_scheduler;
        return the_scheduler;
    }

    scheduler::scheduler()
        : m_poller(new poller),
          m_timer_queue(new timer_queue(*m_poller.get())),
          m_awakener(new awakener(*m_poller.get())) {

    }

    void scheduler::wake_up() {
        m_awakener->wake_up();
    }

    void scheduler::start() {
        if (!m_running) {
            m_running = true;
            run();
        }
    }

    void scheduler::stop() {
        if (m_running) {
            m_running = false;
        }
    }

    void scheduler::post(task_type_ptr &task) {
        m_task_queue.push_back(std::move(task));
        wake_up();
    }

    void scheduler::spawn(task_type_ptr &task) {
        /*auto task(new coro_type::pull_type(task));
        m_task_queue.push_back(task);*/
    }

    void scheduler::run() {
        while (m_running) {
            if(m_cb_before_loop)
                m_cb_before_loop();
            std::vector<std::shared_ptr<event>> active_events;
            m_poller->poll(&active_events, 10);
            for (auto& event : active_events) {
                event->run();
            }
            if(m_cb_after_loop)
                m_cb_after_loop();
        }
    }
}