#pragma once

#include <cstdint>

enum {
    EV_NONE    = 0,
    EV_READ    = 1,
    EV_WRITE   = 1 << 1,
    EV_ONESHOT = 1 << 2,

    EV_ERROR   = 1 << 7
};

class event_mark
{
public:
    event_mark(short mark = 0)
            : m_mark{mark} {
    }

    void enable_read() {
        m_mark |= EV_READ;
    }

    void disable_read() {
        m_mark &= ~EV_READ;
    }

    void enable_write() {
        m_mark |= EV_WRITE;
    }

    void disable_write() {
        m_mark &= ~EV_WRITE;
    }

    void enable_oneshot() {
        m_mask |= EV_ONESHOT;
    }

    void disable_oneshot() {
        m_mask &= ~EV_ONESHOT;
    }

    void disable_all() {
        m_mask = EV_NONE;
    }

    bool is_reading() const {
        return m_mark & EV_READ;
    }

    bool is_writing() const {
        return m_mark & EV_WRITE;
    }

    bool is_oneshot() const {
        return m_mask & EV_ONESHOT;
    }

    bool empty() const {
        return 0 == m_mark;
    }

private:
    uint16_t m_mark;
};
