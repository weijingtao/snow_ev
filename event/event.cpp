//
// Created by weitao on 7/10/16.
//

#include "event.h"


namespace snow
{
    event::event(event&& rhs)
        : m_poller(std::move(rhs.m_poller)),
          m_read_cb(std::move(rhs.m_read_cb)),
          m_write_cb(std::move(rhs.m_write_cb)),
          m_error_cb(std::move(rhs.m_error_cb)),
          m_index(rhs.m_index),
          m_socket_fd(rhs.m_socket_fd),
          m_mask(rhs.m_mask),
          m_ready_mask(rhs.m_ready_mask) {
//        rhs.m_id = 0;
        rhs.m_socket_fd = 0;
        rhs.m_mask = 0;
        rhs.m_ready_mask = 0;
    }


    void event::run() {
        if(is_reading() && m_read_cb)
            m_read_cb();
        if(is_writeable() && m_write_cb)
            m_write_cb();
    }

    void event::update() {

    }
}