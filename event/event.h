//
// Created by weitao on 7/10/16.
//

#pragma once

#include <cstdint>
#include <memory>
#include <functional>
#include <list>

namespace snow
{
    enum {
        EV_NONE    = 0,
        EV_READ    = 1,
        EV_WRITE   = 1 << 1,
        EV_TIMEOUT = 1 << 2,
        EV_ONESHOT = 1 << 3
    };
    class poller;

    class event : public std::enable_shared_from_this<event>
    {
    public:
        typedef std::function<void(void)> event_handle_type;
        typedef std::function<void(int)>  error_handle_type;
        typedef std::list<std::shared_ptr<event>>::const_iterator index_type;

        explicit event(std::shared_ptr<poller>& poller1)
          : m_poller(poller1),
            m_socket_fd(-1),
            m_mask(EV_NONE),
            m_ready_mask(EV_NONE) {

        }

        event()
          : m_socket_fd(-1),
            m_mask(EV_NONE),
            m_ready_mask(EV_NONE) {

        }


        void run();

        void set_poller(std::shared_ptr<poller>& poller1) {
            m_poller = poller1;
        }

        void set_poller(std::shared_ptr<poller>&& poller1) {
            m_poller = std::move(poller1);
        }

        void set_read_cb(event_handle_type&& cb) {
            m_read_cb = std::move(cb);
        }

        void set_write_cb(event_handle_type&& cb) {
            m_write_cb = std::move(cb);
        }

        void set_error_cb(error_handle_type&& cb) {
            m_error_cb = std::move(cb);
        }

        void set_socket_fd(int32_t socket_fd) {
            m_socket_fd = socket_fd;
        }

        int32_t get_socket_fd() const {
            return m_socket_fd;
        }

        void set_mask(uint16_t mask) {
            m_mask = mask;
        }

        uint16_t get_mask() const {
            return m_mask;
        }

        void set_ready_mask(uint16_t ready_mask) {
            m_ready_mask = ready_mask;
        }

        uint16_t get_ready_mask() const {
            return m_ready_mask;
        }

        void enable_reading() {
            m_mask |= EV_READ;
            update();
        }
        void disable_reading() {
            m_mask &= ~EV_READ;
            update();
        }

        void enable_writing() {
            m_mask |= EV_WRITE;
            update();
        }

        void disable_writing() {
            m_mask &= ~EV_WRITE;
            update();
        }

        void enable_oneshot() {
            m_mask |= EV_ONESHOT;
            update();
        }

        void disable_oneshot() {
            m_mask &= ~EV_ONESHOT;
            update();
        }

        void disable_all() {
            m_mask = EV_NONE;
            update();
        }

        bool is_reading() const {
            return m_mask & EV_READ;
        }

        bool is_writing() const {
            return m_mask & EV_WRITE;
        }

        bool is_oneshot() const {
            return m_mask & EV_ONESHOT;
        }

        bool is_readable() const {
            return m_ready_mask & EV_READ;
        }

        bool is_writeable() const {
            return m_ready_mask & EV_WRITE;
        }

        bool is_none_event() const {
            return m_mask == EV_NONE;
        }

    private:
        friend class poller;

        void update();

        void set_index(const index_type& index) {
            m_index = index;
        }

        index_type& get_index() {
            return m_index;
        }

    private:
        std::weak_ptr<poller> m_poller;
        event_handle_type     m_read_cb;
        event_handle_type     m_write_cb;
        error_handle_type     m_error_cb;
        index_type            m_index;
        int32_t               m_socket_fd;
        uint16_t              m_mask;
        uint16_t              m_ready_mask;
    };
}
