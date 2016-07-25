//
// Created by weitao on 7/14/16.
//

#pragma once

#include <cstdint>
#include <string>
#include <tuple>
#include <vector>
#include <locale>

namespace snow
{
class config
{
public:
//    static config& instance();

    config(const std::string& conf_file_name);

    int init(const std::string& conf_file_name);

    int get_proc_num() const {
        return m_proc_num;
    }

    int get_connection_timeout() const {
        return m_connection_timeout;
    }

    int get_max_connection() const {
        return m_max_connecction;
    }

    int get_max_request_per_second() const {
        return m_max_request_per_second;
    }

    int get_log_level() const {
        return m_log_level;
    }

    const std::string& get_log_format() const {
        return m_log_format;
    }

    const std::vector<std::string>& get_endpoints() const {
        return m_endpoints;
    }

private:
    config(const config&) = delete;
    void operator=(const config&) = delete;

    bool is_log_format_valid(const std::string& log_format) const;

    static const int  MIN_PROC_NUM;//                   = 1;
    static const int  DEFAULT_CONNECTION_TIMEOUT;//     = 60 * 1000; //1min
    static const int  MIN_CONNECTION_TIMEOUT;//         = 1000; //1s
    static const int  DEFAULT_MAX_CONNECTION;//         = 100;
    static const int  MIN_CONNECTION;//                 = 1;
    static const int  DEFAULT_MAX_REQUEST_PER_SECOND;// = 1000;
    static const int  DEFAULT_LOG_LEVEL;//              = 3;
    static const int  MIN_LOG_LEVEL;//                  = 0;
    static const int  MAX_LOG_LEVEL;//                  = 6;
    static const char * const DEFAULT_LOG_FORMATE;


private:
    int m_proc_num;
    int m_connection_timeout; //ms
    int m_max_connecction;
    int m_max_request_per_second;
    int m_log_level;
    std::string m_log_format;
    std::vector<std::string> m_endpoints;
};

}
