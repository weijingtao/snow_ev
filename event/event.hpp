//
// Created by weitao on 7/10/16.
//

#pragma once

#include <cstdint>
#include <functional>
#include "event_mark.hpp"


namespace snow
{
    enum {
        EV_NONE    = 0,
        EV_READ    = 1,
        EV_WRITE   = 1 << 1,
        EV_ONESHOT = 1 << 2,

        EV_ERROR   = 1 << 7
    };
    class event
    {
    public:
        typedef std::function<void(int)> event_handle_type;
        static const event_handle_type EMPTY_EVENT_HANDLER;

        event()
          : m_mask(EV_NONE) {
        }

        event(const event&) = delete;

        event& operator=(const event&) = delete;

        void set_read_cb(event_handle_type&& cb) {
            m_read_cb = std::move(cb);
        }

        const event_handle_type& get_read_cb() const {
            return m_read_cb;
        }

        void set_write_cb(event_handle_type&& cb) {
            m_write_cb = std::move(cb);
        }

        const event_handle_type& get_write_cb() const {
            return m_write_cb;
        }

        void set_error_cb(error_handle_type&& cb) {
            m_error_cb = std::move(cb);
        }

        const error_handle_type& get_error_cb() const {
            return m_error_cb;
        }

        void set_mark(uint16_t mark) {
            m_mark = mark;
        }

        uint16_t get_mark() const {
            return m_mark;
        }

        void clear() {
            m_read_cb  = EMPTY_EVENT_HANDLER;
            m_write_cb = EMPTY_EVENT_HANDLER;
            m_error_cb = EMPTY_EVENT_HANDLER;
            m_mark     = EV_NONE;
        }


    private:
        event_handle_type     m_read_cb;
        event_handle_type     m_write_cb;
        event_handle_type     m_error_cb;
        uint16_t              m_mark;
    };

    const event::EMPTY_EVENT_HANDLER;
}
