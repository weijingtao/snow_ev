#pragma

#include <thread>
#include <vector>
#include <functional>

namespace snow
{
    class thread_poll
    {
    public:
        typedef std::function<void(void)> runnable_type;

        thread_poll() = default;

        void start(runnable_type task, std::size_t thread_count = 1);

        void join();

    private:
        std::vector<std::thread> m_threads;
    };
}