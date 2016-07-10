//
// Created by weitao on 7/4/16.
//

#pragma once

#include <cstdint>
#include <vector>
#include <ev.h>
#include "thread_poll.hpp"
#include "buffer.h"
#include "acceptor.h"

namespace snow
{
    class server
    {
    public:
        typedef std::function<void(const buffer&)>      response_dispatch_type;

        server();

        virtual ~server();

        void start();

        void stop();

    protected:
        virtual int init();

        virtual std::size_t pkg_check(const char* data, std::size_t len) = 0;

        virtual void request_dispatch(const char* req_data, std::size_t req_len, response_dispatch_type rsp_dispatcher) = 0;

    private:
        void check();

        void prepare();


    private:
        server(const server&) = delete;
        void operator=(const server&) = delete;

        void run();



    protected:
        bool                    m_stop_flag;
        std::vector<acceptor>   m_acceptors;
        thread_poll             m_thread_poll;
    };
}

