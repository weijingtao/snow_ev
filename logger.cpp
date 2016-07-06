//
// Created by weitao on 7/6/16.
//

#include "logger.h"
#include <iostream>


namespace snow
{
    log_obj::log_obj(log_writer_type &log_writer)
        : m_log_writer(std::move(log_writer)){

    }

    log_obj::log_obj(log_obj &&rhs) {
        using std::swap;
        swap(m_log_writer, rhs.m_log_writer);
        swap(m_buffer, rhs.m_buffer);
    }

    log_obj::~log_obj() {
        m_log_writer(m_buffer.str());
    }

    template <typename T>
    log_obj& log_obj<T>::operator=(const T& value) {
        m_buffer << value;
        return *this;
    }

    logger& logger::instance() {
        static logger log_instance;
        return log_instance;
    }

    logger::logger() {

    }

    log_obj logger::trace() const {
        return log_obj(std::bind(&logger::write, &logger::instance(), log_level::TRACE, std::placeholders::_1));
    }

    log_obj logger::debug() const {
        return log_obj(std::bind(&logger::write, &logger::instance(), log_level::DEBUG, std::placeholders::_1));
    }

    log_obj logger::info() const {
        return log_obj(std::bind(&logger::write, &logger::instance(), log_level::INFO, std::placeholders::_1));
    }

    log_obj logger::warning() const {
        return log_obj(std::bind(&logger::write, &logger::instance(), log_level::WARNING, std::placeholders::_1));
    }

    log_obj logger::error() const {
        return log_obj(std::bind(&logger::write, &logger::instance(), log_level::ERROR, std::placeholders::_1));
    }

    log_obj logger::fatal() const {
        return log_obj(std::bind(&logger::write, &logger::instance(), log_level::FATAL, std::placeholders::_1));
    }

    void logger::write(log_level level, const std::string &str) {
        std::cout << "level[" << reinterpret_cast<int>(level) << "] " << str << std::endl;
    }
}