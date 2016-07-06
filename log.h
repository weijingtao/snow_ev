//
// Created by weitao on 7/6/16.
//

#pragma once

#include <functional>
#include <mutex>
#include <string>

namespace snow
{
    enum class log_level
    {
        TRACE   = 0,
        DEBUG   = 1,
        INFO    = 3,
        WARNING = 4,
        ERROR   = 5,
        FATAL   = 6
    };

    class log
    {
    public:
        typedef std::function<void(const std::string&)> log_call_back_type;

        static log& instance();

        log();

        void trace(const std::string& str);

    private:
        log(const log&) = delete;
        log(log&&) = delete;
        void operator=(const log&) = delete;
        void operator=(log&&) = delete;

        void write(log_level level, const std::string& str);

    private:
        log_call_back_type m_log_cb;
        std::mutex m_mutex;
    };
}
#define SNOW_LOG_TRACE   std::cerr
#define SNOW_LOG_DEBUG   std::cerr
#define SNOW_LOG_INFO    std::cerr
#define SNOW_LOG_WARNING std::cerr
#define SNOW_LOG_ERROR   std::cerr
#define SNOW_LOG_FATAL   std::cerr