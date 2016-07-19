#ifndef _THREAD_POLL_HPP
#define _THREAD_POLL_HPP

#include <thread>
#include <vector>
#include <functional>

namespace snow
{
    class thread_poll
    {
    public:
        typedef std::function<void(void)> task_type;
        thread_poll(std::size_t thread_size)
            : m_threads(thread_size) {

        }

        void set_poll_size(std::size_t thread_count) {
            m_threads.resize(thread_count);
        }

        void start(task_type task) {
            for(auto& thread : m_threads) {
                thread = std::thread(task);
            }
            for(auto& thread : m_threads) {
                thread.join();
            }
        }

        void stop() {
            for(auto& thread : m_threads) {
            }
        }

    private:
        std::vector<std::thread> m_threads;
    };
}

#endif //_THREAD_POLL_HPP