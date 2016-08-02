//
// Created by weitao on 7/1/16.
//

#include "scheduler.h"


#include <cassert>
#include <thread>
#include <array>
#include "event.h"
#include "logger/logger.h"

namespace snow {

    scheduler &scheduler::instance() {
        SNOW_LOG_DEBUG << "thread id:" << std::this_thread::get_id();
        static thread_local scheduler scheduler_instance;
        return scheduler_instance;
    }

    scheduler::scheduler()
        : m_timer_queue(&m_poller),
          m_awakener(&m_poller) {
        SNOW_LOG_DEBUG << "thread id:" << std::this_thread::get_id();
    }

    void scheduler::wake_up() {
        m_awakener.wake_up();
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
        /*if(m_cb_before_loop)
            m_cb_before_loop();*/
        std::vector<event*> active_events;
        m_poller.poll(&active_events, 10000);
        for (auto& event : active_events) {
            SNOW_LOG_DEBUG << "fd:" << event->get_socket_fd() << " mask:" << event->get_mask() << " ready_mask:" << event->get_ready_mask();
            event->run();
        }
        /*if(m_cb_after_loop)
            m_cb_after_loop();*/
    }
}