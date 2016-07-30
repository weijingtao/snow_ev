//
// Created by weitao on 7/4/16.
//

#include "server.h"

#include <cstdio>
#include <cassert>
#include <regex>
#include "net/endpoint.h"
#include "config.h"
#include "utils/net_helper.h"


namespace snow {
    server::server()
        : m_stop_flag(true),
          m_thread_poll(2) {

    }

    server::~server() {
        stop();
    }

    int server::init(const std::string& file_name) {
        config conf(file_name);
        m_max_connecction        = conf.get_max_connection();
        m_connection_timeout     = conf.get_connection_timeout();
        m_max_request_per_second = conf.get_max_request_per_second();
        m_thread_poll.set_poll_size(conf.get_proc_num());

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
        endpoint bind_addr("127.0.0.1", 50000);
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
        run();
    }

    void server::stop() {
        m_stop_flag = true;
    }

    void server::handle_new_connection(connection &) {

    }

    void server::check() {
        for (auto &acceptor : m_acceptors) {
            if (acceptor.try_lock()) {
                acceptor.enable_event_call_back();
                acceptor.unlock();
            }
        }
    }

    void server::prepare() {

    }

    void server::run() {
        /*SNOW_LOG_TRACE << "server runing" << std::endl;
        m_proxy.set_pkg_spliter(std::bind(&server_base::pkg_check, this, std::placeholders::_1, std::placeholders::_2));
        m_proxy.set_request_dispatcher(std::bind(&server_base::request_dispatch, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        while (!m_stop_flag) {
            m_ios.run();
        }*/
    }

    /*void server::request_dispatch(const char *req_data, std::size_t req_len, response_dispatch_type rsp_dispatcher) {
        SNOW_LOG_TRACE << "new request : " << req_data << std::endl;
        auto new_session = std::make_shared<session_type>(m_ios);
        new_session->set_response_dispatcher(std::move(rsp_dispatcher));
        new_session->start(req_data, req_len);
    }*/
}