//
// Created by weitao on 7/10/16.
//

#include "event.h"
#include "../scheduler.h"
#include "../logger/logger.h"

namespace snow
{
    void event::run() {
        if(is_readable() && m_read_cb)
            m_read_cb();
        if(is_writeable() && m_write_cb)
            m_write_cb();
        m_ready_mask = 0;
    }

    void event::update(uint16_t new_mask, uint16_t old_mask) {
        if(m_socket_fd < 0) {
            SNOW_LOG_DEBUG;
        }
        if(new_mask == 0)
            SNOW_LOG_FATAL;
        if(old_mask == 0 && new_mask != 0) {
            SNOW_LOG_DEBUG << "add new event fd:" << m_socket_fd << " interest event:" << m_mask;
//            scheduler::instance().get_poller().add_event(this);
            m_poller->add_event(this);
        } else if(old_mask != 0 && new_mask == 0) {
            SNOW_LOG_DEBUG << "del event fd:" << m_socket_fd << " interest event:" << m_mask;
//            scheduler::instance().get_poller().del_event(this);
            m_poller->del_event(this);
        } else {
            SNOW_LOG_DEBUG << "mod event fd:" << m_socket_fd << " interest event:" << m_mask;
//            scheduler::instance().get_poller().mod_event(this);
            m_poller->mod_event(this);
        }
    }
}