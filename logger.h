//
// Created by weitao on 7/6/16.
//

#pragma once

#include <functional>
#include <mutex>
#include <string>
#include <sstream>
#include <iostream>

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

    class log_obj
    {
    public:
        typedef std::function<void(const std::string&)> log_writer_type;

        explicit log_obj(log_writer_type log_writer);

        log_obj(log_obj&& rhs);

        ~log_obj();

        template <typename T>
        log_obj& operator<<(const T& value) {
            m_buffer << value;
            return *this;
        }

        log_obj& operator<<(const char* value) {
            m_buffer << value;
            return *this;
        }

    private:
        log_obj(const log_obj&) = delete;
        void operator=(const log_obj&) = delete;
        void operator=(const log_obj&&) = delete;

    private:
        log_writer_type   m_log_writer;
        mutable std::stringstream m_buffer;
    };

    class logger
    {
    public:
        typedef std::function<void(const std::string&)> log_writer_type;

        static logger& instance();

        logger();

        log_obj trace() const;

        log_obj debug() const;

        log_obj info() const;

        log_obj warning() const;

        log_obj error() const;

        log_obj fatal() const;

    private:
        logger(const logger&) = delete;
        logger(logger&&) = delete;
        void operator=(const logger&) = delete;
        void operator=(logger&&) = delete;

        void write(log_level level, const std::string& str);

    private:
        class default_log_writer
        {
        public:
            default_log_writer() = default;

            void operator()(const std::string& str);

        private:
            default_log_writer(const default_log_writer&) = delete;
            void operator=(const default_log_writer&) = delete;


        private:
            std::mutex m_mutex;
        };

    private:
        log_writer_type m_log_writer;
        default_log_writer m_default_log_writer;
    };

}

#define SNOW_LOG_TRACE   snow::logger::instance().trace()
#define SNOW_LOG_DEBUG   snow::logger::instance().debug()
#define SNOW_LOG_INFO    snow::logger::instance().info()
#define SNOW_LOG_WARNING snow::logger::instance().warning()
#define SNOW_LOG_ERROR   snow::logger::instance().error()
#define SNOW_LOG_FATAL   snow::logger::instance().fatal()