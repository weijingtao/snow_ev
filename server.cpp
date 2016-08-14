//
// Created by weitao on 7/4/16.
//

#include "server.h"

#include <cstdio>
#include <cassert>
#include <regex>
#include "net/endpoint.h"
#include "config.h"
#include "scheduler.h"
#include "utils/net_helper.h"


namespace snow {
    server::server()
        : m_stop_flag(true) {

    }

    server::~server() {
        stop();
    }

    int server::init(const std::string& file_name) {
        config conf(file_name);
        m_proc_num               = conf.get_proc_num();
        m_max_connecction        = conf.get_max_connection();
        m_connection_timeout     = conf.get_connection_timeout();
        m_max_request_per_second = conf.get_max_request_per_second();
        SNOW_LOG_DEBUG << "proc num :" << m_proc_num
                       << " max connection:" << m_max_connecction
                       << " connection timeout:" << m_connection_timeout
                       << " max request per sencend:" << m_max_request_per_second;

        auto& addrs = conf.get_endpoints();
        for(auto& addr : addrs) {
            SNOW_LOG_DEBUG << addr ;
            std::string interface;
            std::string proto;
            uint32_t port = 0;
            auto pos1 = addr.find_first_of(':');
            if(pos1 != std::string::npos)
                interface = addr.substr(0, pos1);
            auto pos2 = addr.find_first_of('/');
            if(pos2 != std::string::npos)
                port = std::stoi(addr.substr(pos1 + 1, pos2 - pos1));
            proto = addr.substr(pos2 + 1);

            SNOW_LOG_DEBUG << interface << ":" << port << "/" << proto;
            if(port == 0) {
                continue;
            }
            sockaddr sock_addr = {0};
            if(0 != utils::get_local_addr(interface, &sock_addr)) {
                SNOW_LOG_DEBUG;
                continue;
            }
            if(AF_INET == sock_addr.sa_family) {
                reinterpret_cast<sockaddr_in*>(&sock_addr)->sin_port = ::htons(port);
            } else if(AF_INET6 == sock_addr.sa_family) {
                reinterpret_cast<sockaddr_in6*>(&sock_addr)->sin6_port = ::htons(port);
            } else {
                SNOW_LOG_DEBUG;
                continue;
            }
            endpoint bind_addr(sock_addr);
            if(bind_addr) {
                m_acceptors.emplace_back(bind_addr);
            }
        }
        endpoint bind_addr("192.168.89.134", 50000);
        if(bind_addr) {
            m_acceptors.emplace_back(bind_addr);
        }
        assert(!m_acceptors.empty());
        for(auto& acceptor : m_acceptors) {
            acceptor.set_new_connection_handle(std::bind(&server::handle_new_connection, this, std::placeholders::_1));
        }

        return 0;
    }

    void server::start() {
        m_stop_flag = false;
        m_proc_num = 4;
        m_thread_poll.start(std::bind(&server::run, this), m_proc_num);
        m_thread_poll.join();
    }

    void server::stop() {
        m_stop_flag = true;
    }

    void server::handle_new_connection(std::unique_ptr<connection>& conn) {
        SNOW_LOG_DEBUG << "new connection fd";
        conn->set_close_handler(std::bind(&server::handle_close, this, std::placeholders::_1));
        conn->set_pkg_spliter(std::bind(&server::pkg_check, this, std::placeholders::_1, std::placeholders::_2));
        conn->set_dispatcher(std::bind(&server::request_dispatch, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        conn->set_timeout(m_connection_timeout);
//        conn->start();
        auto it = m_connections.insert(m_connections.cend(), std::move(conn));
        (*it)->set_index(it);
        (*it)->start();
    }

    void server::handle_close(connection::index_type &index) {
        m_connections.erase(index);
    }

    void server::check() {
        SNOW_LOG_DEBUG << "check acceptor num:" << m_acceptors.size();
        for (auto &acceptor : m_acceptors) {
            if (acceptor.try_lock()) {
                SNOW_LOG_FATAL << "thread[" << std::this_thread::get_id() << "] get the mutex";
                acceptor.enable_event_call_back();
            }
        }
    }

    void server::prepare() {
        SNOW_LOG_DEBUG;
    }

    void server::run() {
        while(!m_stop_flag) {
            SNOW_LOG_TRACE << "thread[" << std::this_thread::get_id() << "] server runing";
            check();
            scheduler::instance().run();
            prepare();
        };
    }
}