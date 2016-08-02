//
// Created by weitao on 7/9/16.
//

#pragma once

#include <sys/epoll.h>
#include <sys/time.h>
#include <unistd.h>
#include <cstdint>
#include <memory>
#include <array>
#include <vector>
#include <list>
#include "event.h"

namespace snow
{

    class poller
    {
    public:
        typedef std::list<std::shared_ptr<event>> event_list;
        typedef event_list::const_iterator        event_id;

        poller();

        ~poller();

        int add_event(event* ev);

        int mod_event(event* ev);

        int del_event(event* ev);

        void poll(std::vector<event*>* read_event, uint32_t time_out);



    private:
        poller(const poller&) = delete;
        void operator=(const poller&) = delete;

    private:
        event_list               m_events;
        std::vector<epoll_event> m_active_events;
        std::size_t              m_active_event_count;
        int                      m_epoll_fd;
    };
}
