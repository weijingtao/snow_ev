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
#include "socket_fd.hpp"
#include "event.h"
#include "../logger/logger.h"

namespace snow
{

    class poller
    {
    public:
        typedef std::function<void (int)> call_back_type;

        poller()
                : m_epoll_fd{::epoll_create1(::EPOLL_CLOEXEC)} {
            assert(m_epoll_fd);
        }

        ~poller(){
            if(m_epoll_fd) {
                ::close(m_epoll_fd);
                m_epoll_fd = -1;
            }
        }

        poller(const poller&) = delete;

        poller& operator=(const poller&) = delete;

        int add_event(const socket_fd& fd, const event_mark& mark, const call_back_type& cb) {
            assert(fd && mark.empty() && cb);
            if(fd >= MAX_FD) {
                return;
            }
            SNOW_LOG_DEBUG << "add socket fd " << fd << " mask " << mask;
            auto old_mark{m_events[fd].get_mark()};
            struct epoll_event ev_item{0};
            if (mark.is_reading())
                ev_item.events |= ::EPOLLIN;
            m_events[fd]    = cb;
            if (mark.is_writing())
                ev_item.events |= ::EPOLLOUT;
            m_events[fd]    = cb;
            if (mark.is_oneshot())
                ev_item.events |= ::EPOLLONESHOT;
            if(old_mark == mark) {
                return 0;
            }
            ev_item.fd = fd;
            int op{EPOLL_CTL_ADD};
            if(!old_mark.empty() && old_mark != mark) {
                op = EPOLL_CTL_MOD;
            }
            SNOW_LOG_DEBUG << "EPOLL_CTL_ADD mask " << ev_item.events;
            if (epoll_ctl(m_epoll_fd, op, fd, &ev_item) == 0) {
                SNOW_LOG_DEBUG << "socket fd " << ev->get_socket_fd() << " add event success";
                return 0;
            }
        }

        int del_event(const socket_fd& fd, const event_mark& mark) {
            SNOW_LOG_DEBUG << "del socket fd " << ev->get_socket_fd() << " event addr " << ev;
            if(ev->get_socket_fd() < 0) {
                return -1;
            }
            if(!ev->is_none_event()) {
                return -1;
            }

            struct epoll_event ev_item;
            std::memset(&ev_item, 0, sizeof ev_item);
            ev_item.data.ptr = ev;
            if (::epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, ev->get_socket_fd(), &ev_item) == 0) {
                return 0;
            } else {
                return -1;
            }
        }

        int poll(std::vector<call_back_type> * ready_events, int timeout) {
            assert(ready_event != nullptr);
            std::vector<epoll_event> events;
            events.reserve(/*m_events.size()*/10);
            int active_events = ::epoll_wait(m_epoll_fd, &events[0], events.size(), time_out);
            if(active_events < 0) {
                SNOW_LOG_FATAL << "epoll fd " << m_epoll_fd << " errno:" << errno << ", errmsg " << strerror(errno);
            }
//        std::cout << "active_events : " << active_events << std::endl;
            if (active_events > 0) {
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
        }


    private:
        static const std::size_t MAX_FD = 4096;
        static std::array<event, MAX_FD> m_events;
        socket_fd                m_epoll_fd;

    };
}
