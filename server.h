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

        server();

        virtual ~server();

        void start();

        void stop();

    protected:
        virtual int init();

        virtual std::size_t pkg_check(const char* data, std::size_t len) = 0;


    private:
        server(const server&) = delete;
        void operator=(const server&) = delete;

        void run();

        void request_dispatch(const char* req_data, std::size_t req_len, response_dispatch_type rsp_dispatcher);

    protected:
        bool                    m_stop_flag;
        std::vector<acceptor>   m_acceptors;
        thread_poll             m_thread_poll;
    };
}


#endif //SNOW_LIBEV_SERVER_H
