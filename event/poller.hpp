//
// Created by weitao on 7/9/16.
//

#pragma once

#include <sys/epoll.h>
#include <sys/time.h>
#include <unistd.h>
#include <cassert>
#include <cstring>
#include <iostream>
#include <cstdint>
#include <memory>
#include <array>
#include <vector>
#include <list>
#include <functionnal>
#include "event.hpp"
#include "../logger/logger.h"

namespace snow
{

    class poller
    {
    public:
        typedef std::function<void (int)> call_back_type;

        poller()
                : m_epoll_fd{::epoll_create1(::EPOLL_CLOEXEC)} {
            assert(m_epoll_fd >= 0);
        }

        ~poller() {
            if(m_epoll_fd >= 0) {
                ::close(m_epoll_fd);
            }
        }

        poller(const poller&) = delete;

        poller& operator=(const poller&) = delete;

        int add_event(int fd, uint16_t mark, const call_back_type& cb) {
            assert(fd >= 0 && mark != EV_NONE && cb);
            if(fd < 0 || fd >= MAX_FD) {
                return -1;
            }
            SNOW_LOG_DEBUG << "add socket fd " << fd << " mask " << mask;

            if (mark & EV_READ) { m_events[fd].set_read_cb(cb); }
            if (mark & EV_WRITE) { m_events[fd].get_write_cb(cb); }
            if(mark & EV_ERROR) { m_events[fd].set_error_cb(cb); }

            if(mark = m_events[fd].get_mark()) {
                return 0;
            }

            int op = m_events[fd].get_mark() == EV_NONE ? EPOLL_CTL_ADD : EPOLL_CTL_MOD;
            m_events[fd].set_mark(m_events[fd].get_mark() | mark);

            struct epoll_event ev_item{0};
            ev_item.fd = fd;
            if (m_events[fd].get_mark() & EV_READ) { ev_item.events |= ::EPOLLIN; }
            if (m_events[fd].get_mark() & EV_WRITE) { ev_item.events |= ::EPOLLOUT; }
            if (m_events[fd].get_mark() & EV_ONESHOT) { ev_item.events |= ::EPOLLONESHOT; }

            SNOW_LOG_DEBUG << "EPOLL_CTL_ADD mask " << ev_item.events;
            if (epoll_ctl(m_epoll_fd, op, fd, &ev_item) == 0) {
                SNOW_LOG_DEBUG << "socket fd " << ev->get_socket_fd() << " op " << op << " event " << ev_item.events << " add event success";
                return 0;
            } else {
                SNOW_LOG_ERROR << "socket fd " << ev->get_socket_fd() << " op " << op << " event " << ev_item.events << " add event fail";
                return -2;
            }
        }

        int del_event(int fd, uint16_t mark) {
            assert(fd >= 0 && mark != EV_NONE);
            SNOW_LOG_DEBUG << "del socket fd " << fd << " mark " << mark;
            if(fd < 0 || fd > MAX_FD) {
                return -1;
            }
            if(EV_NONE == mark || EV_NONE == m_events[fd].get_mark()) {
                return -2;
            }
            struct epoll_event ev_item{0};
            ev_item.fd = fd;
            uint16_t new_mark = mark & ~m_events[fd].get_mark();
            int op = EPOLL_CTL_MOD;
            if(new_mark & (EV_READ | EV_WRITE)) {
                m_events[fd].clear();
                op = EPOLL_CTL_DEL;
            } else {
                if(m_events[fd].get_mark() & EV_READ && !(new_mark & EV_NONE)) {
                    m_events[fd].set_read_cb(event::EMPTY_EVENT_HANDLER);
                }
                if(m_events[fd].get_mark() & EV_WRITE && !(new_mark & EV_WRITE)) {
                    m_events[fd].set_write_cb(event::EMPTY_EVENT_HANDLER);
                }
                if (m_events[fd].get_mark() & EV_READ) { ev_item.events |= ::EPOLLIN; }
                if (m_events[fd].get_mark() & EV_WRITE) { ev_item.events |= ::EPOLLOUT; }
                if (m_events[fd].get_mark() & EV_ONESHOT) { ev_item.events |= ::EPOLLONESHOT; }
            }

            if (::epoll_ctl(m_epoll_fd, op, fd, &ev_item) == 0) {
                SNOW_LOG_DEBUG << "socket fd " << ev->get_socket_fd() << " op " << op << " event " << ev_item.events << " del event success";
                return 0;
            } else {
                SNOW_LOG_ERROR << "socket fd " << ev->get_socket_fd() << " op " << op << " event " << ev_item.events << " add event fail";
                return -3;
            }
        }

        int poll(std::vector<call_back_type> * ready_events, int timeout) {
            assert(ready_event != nullptr);
            std::vector<epoll_event> events;
            events.reserve(/*m_events.size()*/10);
            int active_events = ::epoll_wait(m_epoll_fd, &events[0], events.size(), time_out);
            if(active_events < 0) {
                SNOW_LOG_FATAL << "epoll fd " << m_epoll_fd << " errno:" << errno << ", errmsg " << strerror(errno);
                return -1;
            }
            for (int index = 0; index < active_events; ++index) {
                SNOW_LOG_DEBUG << "EPOLLIN:" << EPOLLIN << " EPOLLOUT:" << EPOLLOUT << " EPOLLERR:" << EPOLLERR << " EPOLLHUP:" << EPOLLHUP;
                SNOW_LOG_DEBUG << "epoll event:" << events[index].events;
                if (events[index].events & EPOLLIN) {
                    ready_events->emplace_back(std::bind(m_events[events[index].fd].get_read_cb(), EV_READ));
                }
                if(events[index].events & EPOLLOUT) {
                    ready_events->emplace_back(std::bind(m_events[events[index].fd].get_write_cb(), EV_WRITE));
                }
                if (events[index].events & (EPOLLERR | EPOLLHUP)) {
                    ready_events->emplace_back(std::bind(m_events[events[index].fd].get_error_cb(), EV_ERROR));
                }
            }
        }


    private:
        static const std::size_t MAX_FD = 4096;
        static std::array<event, MAX_FD> m_events;
        int                              m_epoll_fd;

    };
}
