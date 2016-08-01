//
// Created by weitao on 8/1/16.
//

#pragma once

#include <memory>
#include <functional>
#include "timer/timer.h"
#include "net/buffer.h"

namespace snow
{
    class session : public std::enable_shared_from_this<session>
    {
    public:
        typedef std::function<void(const buffer&)> response_handle_type;

        void set_response_handler(const response_handle_type& handler) {
            m_response_handler = handler;
        }

        void set_timeout(std::size_t timeout) {

        }

        void send_to_client(const char* data, std::size_t len) {
            if(m_response_handler) {
                m_response_handler(buffer(data, len));
            }
        }

        virtual void process(const char* data, std::size_t len) = 0;

    private:
        timer m_timer;
        response_handle_type m_response_handler;
    };
}
