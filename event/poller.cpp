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
#include "../logger/logger.h"


namespace snow
{
    poller::poller()
      : m_epoll_fd(-1),
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

    int poller::add_event(event* ev) {
        SNOW_LOG_DEBUG;
        if(ev->get_socket_fd() < 0) {
            return -1;
        }
        if(ev->is_none_event()) {
            return -1;
        }
        struct epoll_event ev_item;
        std::memset(&ev_item, 0, sizeof ev_item);
        uint32_t mask = 0;
        if (ev->is_reading())
            ev_item.events |= ::EPOLLIN;
        if (ev->is_writing())
            ev_item.events |= ::EPOLLOUT;
        if (ev->is_oneshot())
            ev_item.events |= ::EPOLLONESHOT;
        ev_item.data.ptr = ev;

        if (epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, ev->get_socket_fd(), &ev_item) == 0) {
//            auto it = m_events.insert(m_events.cbegin(), event);
//            event->set_index(it);
            return 0;
        } else {
            return -1;
        }
    }

    int poller::mod_event(event* ev) {
        if(ev->get_socket_fd() < 0) {
            return -1;
        }
        if(ev->is_none_event()) {
            return -1;
        }
        struct epoll_event ev_item;
        std::memset(&ev_item, 0, sizeof ev_item);
        uint32_t mask = 0;
        if (ev->is_reading())
            ev_item.events |= ::EPOLLIN;
        if (ev->is_writing())
            ev_item.events |= ::EPOLLOUT;
        if (ev->is_oneshot())
            ev_item.events |= ::EPOLLONESHOT;
        ev_item.data.ptr = ev;

        if (epoll_ctl(m_epoll_fd, EPOLL_CTL_MOD, ev->get_socket_fd(), &ev_item) == 0) {
            return 0;
        } else {
            return -1;
        }
    }


    int poller::del_event(event* ev) {
        if(ev->get_socket_fd() < 0) {
            return -1;
        }
        if(!ev->is_none_event()) {
            return -1;
        }
        struct epoll_event ev_item;
        std::memset(&ev_item, 0, sizeof ev_item);
        ev_item.data.ptr = ev;
        if (epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, ev->get_socket_fd(), &ev_item) == 0) {
            m_events.erase(ev->get_index());
            return 0;
        } else {
            return -1;
        }
    }

    void poller::poll(std::vector<event*>* ready_event, uint32_t time_out) {
        assert(ready_event != nullptr);
        std::vector<epoll_event> events;
        events.resize(/*m_events.size()*/10);
        int active_events = epoll_wait(m_epoll_fd, &events[0], events.size(), time_out);
        if(active_events < 0) {
            SNOW_LOG_FATAL << "epoll fd " << m_epoll_fd << " errno:" << errno << ", errmsg " << strerror(errno);
        }
        std::cout << "active_events : " << active_events << std::endl;
        if (active_events > 0) {
            for (int index = 0; index < active_events; ++index) {
                SNOW_LOG_DEBUG << "EPOLLIN:" << EPOLLIN << " EPOLLOUT:" << EPOLLOUT << " EPOLLERR:" << EPOLLERR << " EPOLLHUP:" << EPOLLHUP;
                SNOW_LOG_DEBUG << "epoll event:" << events[index].events;
                uint32_t ready_mask = 0;
                if (events[index].events & EPOLLIN)
                    ready_mask |= EV_READ;
                if (events[index].events & (EPOLLOUT | EPOLLERR | EPOLLHUP))
                    ready_mask |= EV_WRITE;
                auto* event_ptr = static_cast<event*>(events[index].data.ptr);
                event_ptr->set_ready_mask(ready_mask);
                ready_event->push_back(event_ptr);
            }
        }
    }
}