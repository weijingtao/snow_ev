//
// Created by weitao on 7/10/16.
//

#pragma once

#include <cstdint>
#include <functional>
#include "event_mark.hpp"


namespace snow
{

    class event
    {
    public:
        typedef std::function<void(int)> event_handle_type;

        event()
          : m_mask(EV_NONE),
            m_ready_mask(EV_NONE) {
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

        void set_mask(uint16_t mask) {
            m_mask = mask;
        }

        event_mark get_mask() const {
            return m_mask;
        }

        void clear() {
            m_read_cb  = EMPTY_EVENT_HANDLER;
            m_write_cb = EMPTY_EVENT_HANDLER;
            m_error_cb = EMPTY_EVENT_HANDLER;
            m_mark.disable_all();
        }


    private:
        static const event_handle_type EMPTY_EVENT_HANDLER;
        event_handle_type     m_read_cb;
        event_handle_type     m_write_cb;
        event_handle_type     m_error_cb;
        event_mark            m_mark;
    };

    const event::EMPTY_EVENT_HANDLER;
}
