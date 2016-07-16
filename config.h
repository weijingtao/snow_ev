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
    static config& instance();

    config();

    int init(const std::string& conf_file_name);

private:
    config(const config&) = delete;
    void operator=(const config&) = delete;

    bool is_log_format_valid(const std::string& log_format) const;

    static const std::size_t  MIN_PROC_NUM                   = 1;
    static const std::size_t  DEFAULT_CONNECTION_TIMEOUT     = 60 * 1000; //1min
    static const std::size_t  MIN_CONNECTION_TIMEOUT         = 1000; //1s
    static const std::size_t  DEFAULT_MAX_CONNECTION         = 100;
    static const std::size_t  MIN_CONNECTION                 = 1;
    static const std::size_t  DEFAULT_MAX_REQUEST_PER_SECOND = 1000;
    static const std::size_t  DEFAULT_LOG_LEVEL              = 3;
    static const std::size_t  MIN_LOG_LEVEL                  = 0;
    static const std::size_t  MAX_LOG_LEVEL                  = 6;
    static const char * const DEFAULT_LOG_FORMATE;


private:
    std::size_t m_proc_num;
    std::size_t m_connection_timeout; //ms
    std::size_t m_max_connecction;
    std::size_t m_max_request__per_second;
    std::size_t m_log_level;
    std::string m_log_format;
    std::vector<std::string> m_endpoints;
};

}
