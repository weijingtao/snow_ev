//
// Created by weitao on 7/4/16.
//

#ifndef SNOW_LIBEV_SERVER_H
#define SNOW_LIBEV_SERVER_H


#include <cstdint>
#include <vector>
#include "proxy.hpp"
#include "thread_poll.hpp"
#include "buffer.h"
#include "acceptor.h"

namespace snow
{
    template <typename session_type>
    class server
    {
    public:
        typedef std::function<void(const buffer&)>      response_dispatch_type;

        server()
            : m_stop_flag(true),
              m_proxy(m_ios),
              m_thread_poll(2) {

        }

        virtual ~server() {
            stop();
        }

        void start() {
            m_stop_flag = false;
            m_thread_poll.start(std::bind(&server_base::run, this));
        }

        void stop() {
            m_stop_flag = true;
        }

    protected:
        virtual int init() {
            return 0;
        }

        virtual std::size_t pkg_check(const char* data, std::size_t len) = 0;


    private:
        server(const server&) = delete;
        void operator=(const server&) = delete;

        void run() {
            SNOW_LOG_TRACE << "server runing" << std::endl;
            m_proxy.set_pkg_spliter(std::bind(&server_base::pkg_check, this, std::placeholders::_1, std::placeholders::_2));
            m_proxy.set_request_dispatcher(std::bind(&server_base::request_dispatch, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
            while(!m_stop_flag) {
                m_ios.run();
            }
        }

        void request_dispatch(const char* req_data, std::size_t req_len, response_dispatch_type rsp_dispatcher) {
            SNOW_LOG_TRACE <<  "new request : " << req_data << std::endl;
            auto new_session = std::make_shared<session_type>(m_ios);
            new_session->set_response_dispatcher(std::move(rsp_dispatcher));
            new_session->start(req_data, req_len);
        }

    protected:
        bool                    m_stop_flag;
        std::vector<acceptor>   m_acceptors;
        proxy                   m_proxy;
        thread_poll             m_thread_poll;
    };
}


#endif //SNOW_LIBEV_SERVER_H
