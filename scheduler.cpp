//
// Created by weitao on 7/1/16.
//

#include "scheduler.h"

#include <unistd.h>
#include <cassert>
#include <thread>
#include <array>
#include "context.h"

namespace snow {

    scheduler &scheduler::instance() {
        static thread_local scheduler the_scheduler;
        return the_scheduler;
    }

    scheduler::scheduler()
        : m_loop(ev_loop_new()),
          m_io_watcher(new ev_io) {

    }

    void scheduler::init() {
        if (0 == ::socketpair(AF_LOCAL, ::SOCK_DGRAM, 0, m_socket_pair)) {
            ev_io_init(m_io_watcher.get(), scheduler::ev_io_call_back, m_socket_pair[1], EV_READ);
            m_io_watcher->data = static_cast<void *>(this);
            ev_io_start(m_loop.get(), m_io_watcher.get());
        }
    }

    void scheduler::ev_io_call_back(struct ev_loop *loop, ev_io *io_watcher, int revents) {
        assert(nullptr != io_watcher->data);
        static_cast<scheduler *>(io_watcher->data)->wake_up();
    }

    void scheduler::ev_timer_call_back(struct ev_loop *loop, ev_timer *watcher) {
        assert(nullptr != watcher->data);
    }

    void scheduler::wake_up() {
        std::array<char, 32> buffer;
        std::size_t n_read = 0;
        while ((n_read = ::read(m_socket_pair[1], buffer.data(), buffer.size())) > 0);
        while (!m_task_queue.empty()) {
            task_type_ptr task = std::move(m_task_queue.front());
            (*task)();
            m_task_queue.pop_front();
        }

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
        m_task_queue.push_back(task);
        ::write(m_socket_pair[0], "#", 1);
    }

    void scheduler::spawn(task_type_ptr &task) {
        /*auto task(new coro_type::pull_type(task));
        m_task_queue.push_back(task);*/
    }

    void scheduler::run() {
        while (m_running) {
            ev_run(m_loop.get(), 0);
        }
    }
}