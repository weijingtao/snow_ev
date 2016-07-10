//
// Created by weitao on 7/10/16.
//

#include "event.h"


namespace snow
{
    void event::run() {
        if(is_reading() && m_read_cb)
            m_read_cb();
        if(is_writeable() && m_write_cb)
            m_write_cb();
    }

    void event::update() {

    }
}