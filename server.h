//
// Created by weitao on 7/4/16.
//

#pragma once

#include <cstdint>
#include <vector>
#include <list>
#include <string>
#include <functional>
#include "thread_poll.h"
#include "scheduler.h"
#include "net/buffer.h"
#include "net/acceptor.h"

namespace snow
{
    class server
    {
    public:
        typedef std::function<void(const buffer&)>      response_dispatch_type;

        server();

        virtual ~server();

        int init(const std::string&);

        void start();

        void stop();

    protected:

        virtual std::size_t pkg_check(const char* data, std::size_t len) = 0;

        virtual void request_dispatch(const char* req_data, std::size_t req_len, response_dispatch_type rsp_dispatcher) = 0;

    private:
        void handle_new_connection(std::unique_ptr<connection>&);

        void handle_close(connection::index_type& index);

        void check();

        void prepare();


    private:
        server(const server&) = delete;
        void operator=(const server&) = delete;

        void run();



    protected:
        bool                    m_stop_flag;
        std::list<acceptor>     m_acceptors;
        thread_poll             m_thread_poll;
        std::list<scheduler>    m_schedulers;
        std::list<std::unique_ptr<connection>>   m_connections;
        int m_proc_num;
        int m_connection_timeout; //ms
        int m_max_connecction;
        int m_max_request_per_second;
    };

    template <typename SessionType>
    class server_base : public server
    {
    public:
        virtual void request_dispatch(const char* req_data, std::size_t req_len, response_dispatch_type rsp_dispatcher) {
            auto session = std::make_shared<SessionType>();
            session->set_response_handler(rsp_dispatcher);
            session->process(req_data, req_len);
//            m_sessions.push_back(session);
        }

    private:
        typedef std::shared_ptr<SessionType> session_ptr;
        std::vector<session_ptr> m_sessions;
    };
}

