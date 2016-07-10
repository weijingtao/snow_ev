//
// Created by weitao on 7/9/16.
//

//
// Created by weitao on 4/2/16.
//

#include "poller.h"

#include <cassert>
#include <cstring>
#include <iostream>


namespace snow
{
    poller::poller()
        : m_epoll_fd(-1),
          m_active_events,
          m_active_event_count(0) {
        m_epoll_fd = epoll_create1(::EPOLL_CLOEXEC);
        assert(m_epoll_fd > 0);
    }

    poller::~poller(){
        if(m_epoll_fd >= 0) {
            ::close(m_epoll_fd);
            m_epoll_fd = -1;
        }
    }

    int poller::add_event(std::shared_ptr<event>& event) {
        if(event->get_socket_fd() < 0) {
            return -1;
        }
        if(event->is_none_event()) {
            return -1;
        }
        struct epoll_event ev;
        std::memset(&ev, 0, sizeof ev);
        uint32_t mask = 0;
        if (event->is_reading())
            ev.events |= ::EPOLLIN;
        if (event->is_writing())
            ev.events |= ::EPOLLOUT;
        if (event->is_oneshot())
            ev.events |= ::EPOLLONESHOT;
        ev.data.ptr = event.get();

        if (epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, event->get_socket_fd(), &ev) == 0) {
            auto it = m_events.insert(m_events.cbegin(), event);
            event->set_index(it);
            return 0;
        } else {
            return -1;
        }
    }

    int poller::mod_event(std::shared_ptr<event> &event) {
        if(event->get_socket_fd() < 0) {
            return -1;
        }
        if(event->is_none_event()) {
            return -1;
        }
        struct epoll_event ev;
        std::memset(&ev, 0, sizeof ev);
        uint32_t mask = 0;
        if (event->is_reading())
            ev.events |= ::EPOLLIN;
        if (event->is_writing())
            ev.events |= ::EPOLLOUT;
        if (event->is_oneshot())
            ev.events |= ::EPOLLONESHOT;
        ev.data.ptr = event.get();

        if (epoll_ctl(m_epoll_fd, EPOLL_CTL_MOD, event->get_socket_fd(), &ev) == 0) {
            return 0;
        } else {
            return -1;
        }
    }


    int poller::del_event(std::shared_ptr<event> &event) {
        if(event->get_socket_fd() < 0) {
            return -1;
        }
        if(!event->is_none_event()) {
            return -1;
        }
        struct epoll_event ev;
        std::memset(&ev, 0, sizeof ev);
        ev.data.ptr = event.get();
        if (epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, event->get_socket_fd(), &ev) == 0) {
            m_events.erase(event->get_index());
            return 0;
        } else {
            return -1;
        }
    }

    void poller::poll(std::vector<std::shared_ptr<event>>* ready_event, uint32_t time_out) {
        assert(ready_event != nullptr);
        ready_event->resize(m_events.size());
        int active_events = epoll_wait(m_epoll_fd, &(*ready_event)[0], ready_event->size(), time_out);
        std::cout << "active_events : " << active_events << std::endl;
        if (active_events > 0) {
            for (int index = 0; index < active_events; ++index) {
                auto& ev = (*ready_event)[index];

                uint32_t ready_mask = 0;
                if (ev->events & EPOLLIN)
                    ready_mask |= EV_READ;
                if (ev->events & (EPOLLOUT | EPOLLERR | EPOLLHUP))
                    ready_mask |= EV_WRITE;
                auto* event_ptr = static_cast<event*>(ev.data.ptr);
                event_ptr->set_ready_mask(ready_mask);
                ready_event->push_back(event_ptr->shared_from_this());
                if (event_ptr->is_oneshot()) {
                    m_events.erase(event_ptr->get_index());
                }
            }
        }
    }
}