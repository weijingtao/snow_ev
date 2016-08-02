//
// Created by weitao on 8/1/16.
//

#include "thread_poll.h"

namespace snow
{
    void thread_poll::start(runnable_type task, std::size_t thread_count) {
        for(std::size_t i = 0; i < thread_count; ++i) {
            m_threads.emplace_back(task);
        }
    }

    void thread_poll::join() {
        for(auto& thread : m_threads) {
            thread.join();
        }
    }
}