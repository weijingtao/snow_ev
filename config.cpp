//
// Created by weitao on 7/14/16.
//

#include "config.h"

#include <sys/sysinfo.h>
#include <exception>
#include <algorithm>
#include "yaml-cpp/yaml.h"
#include "logger/logger.h"

namespace snow
{
    const int  config::MIN_PROC_NUM                   = 1;
    const int  config::DEFAULT_CONNECTION_TIMEOUT     = 60 * 1000; //1min
    const int  config::MIN_CONNECTION_TIMEOUT         = 1000; //1s
    const int  config::DEFAULT_MAX_CONNECTION         = 100;
    const int  config::MIN_CONNECTION                 = 1;
    const int  config::DEFAULT_MAX_REQUEST_PER_SECOND = 1000;
    const int  config::DEFAULT_LOG_LEVEL              = 3;
    const int  config::MIN_LOG_LEVEL                  = 0;
    const int  config::MAX_LOG_LEVEL                  = 6;
    const char* const config::DEFAULT_LOG_FORMATE = "";

    /*config& config::instance() {
        static config config_instance;
        return config_instance;
    }*/

    config::config(const std::string& conf_file_name)
        : m_proc_num(::get_nprocs()),
          m_connection_timeout(DEFAULT_CONNECTION_TIMEOUT),
          m_max_connecction(DEFAULT_MAX_CONNECTION),
          m_max_request_per_second(DEFAULT_MAX_REQUEST_PER_SECOND),
          m_log_level(DEFAULT_LOG_LEVEL),
          m_log_format(DEFAULT_LOG_FORMATE) {
        init(conf_file_name);
    }

    int config::init(const std::string &conf_file_name) {
        try {
            YAML::Node config = YAML::LoadFile("config.yaml");
            if(config["interface"] && config["interface"].IsSequence())
            {
                for(auto it = config["interface"].begin(); it != config["interface"].end(); ++it) {
                    m_endpoints.emplace_back(it->as<std::string>());
                }
            }

            if(config["service"] && config["service"].IsMap())
            {
                if(config["service"]["procnum"]) {
                    m_proc_num = std::max(m_proc_num, config["service"]["procnum"].as<int>());
                }
                if(config["service"]["log"] && config["service"]["log"].IsMap())
                {
                    if(config["service"]["log"]["level"]) {
                        m_log_level = std::max(config::MIN_LOG_LEVEL, config["service"]["log"]["level"].as<int>());
                        m_log_level = std::min(m_log_level, config["service"]["log"]["level"].as<int>());
                    }
                    if(config["service"]["log"]["format"] && is_log_format_valid(config["service"]["log"]["format"].as<std::string>())) {
                        m_log_format = config["service"]["log"]["format"].as<std::string>();
                    }
                }
            }

            if(config["limits"] && config["limits"].IsMap())
            {
                if(config["limits"]["max_connection"]) {
                    m_max_connecction = std::max(MIN_CONNECTION, config["limits"]["max_connection"].as<int>());
                }
                if(config["limits"]["connection_timeout"]) {
                    m_connection_timeout = std::max(MIN_CONNECTION_TIMEOUT, config["limits"]["connection_timeout"].as<int>());
                }
                if(config["limits"]["tcp_send_buf"])
                    std::cout << config["limits"]["tcp_send_buf"].as<int>() << std::endl;
                if(config["limits"]["tcp_recv_buf"])
                    std::cout << config["limits"]["tcp_recv_buf"].as<int>() << std::endl;
            }
            return 0;
        } catch (std::exception& e) {
            SNOW_LOG_FATAL << "config init failed : " << e.what();
            return -1;
        }
    }

    bool config::is_log_format_valid(const std::string& log_format) const {
        return true;
    }
}